#include "system_calls.h"
#include "file_system.h"
#include "terminal.h"
#include "rtc.h"
#include "paging.h"
#include "x86_desc.h"
#include "idt.h"

uint8_t magic[4] = {0x7f, 0x45, 0x4c, 0x46};

// uint32_t num_shells;

/* int32_t system_call_init()
 * Inputs: none
 * Return Value: result
 * Function: inits system calls */
int32_t system_call_init(){
    cli();
    process_count = 0;
    active_terminal = 0;   

    // set up stdin and stdout
    stdin_ops.read = &terminal_read;
    stdout_ops.write = &terminal_write;

    // rtc operations
    RTC_ops.open = &open_rtc;
    RTC_ops.close = &close_rtc;
    RTC_ops.read = &read_rtc;
    RTC_ops.write = &write_rtc;

    // file operations
    file_ops.open = &file_open;
    file_ops.close = &file_close;
    file_ops.read = &file_read;
    file_ops.write = &file_write;

    // directory operations
    directory_ops.open = &directory_open;
    directory_ops.close = &directory_close;
    directory_ops.read = &directory_read;
    directory_ops.write = &directory_write;

    // init pcb index
    memset(pcb_index, 0, sizeof(uint32_t)*NUM_TERM);
    // pcb_index = 0;

    // number of shells
    // num_shells = 0;

    char com[COMMAND_LEN] = "shell";

    dentry_t dentry;
    pcb_t* current_pcb;

    int i;

    char file_start[MAGIC];
    // get keyword's dentry
    if(read_dentry_by_name((uint8_t*)com, &dentry))
        return -1;

    // get the first 28 bytes of the keyword
    if(read_data(dentry.inode_num, 0, (uint8_t*)file_start, 28) != 28)
        return -1;
    // determine if the file has magic numbers
    if(strncmp((int8_t*)file_start, (int8_t*)magic, (uint32_t)4))
        return -1;

    // reverse order to start terminal 1 first
    for(i=NUM_TERM-1; i>=0; i--){
        // find the next avail location for pcb
        if(process_count < MAX_PCB){
            current_pcb = get_current_pcb(i, pcb_index[i]);
            current_pcb->PID = pcb_index[i];
            store_args(com, current_pcb); //added this to call stroing args in pcb
            current_pcb->EBP = get_current_pcb_ebp(i, pcb_index[i]); // EIGHT_MB - (pcb_index[i] * EIGHT_KB) - (EIGHT_KB * 4 * active_terminal) - 4;
            set_program_page(pcb_index[i] + i*4);
            
            current_pcb->parent_PID = -1;

            pcb_index[i]++;
            process_count++;
        } else
            return -1;

        // save the value of the EIP
        current_pcb->EIP = ((uint32_t*)file_start)[6];

        // add stdin and stdout for fdt
        current_pcb->file_descriptor_table[0].entries = &stdin_ops;
        current_pcb->file_descriptor_table[0].flags = 1;

        current_pcb->file_descriptor_table[1].entries = &stdout_ops;
        current_pcb->file_descriptor_table[1].flags = 1;

        // indcates no vidmap by default
        current_pcb->vidmap_flag = 0;

        // sets default values for last context
        current_pcb->last_EBP = 0;
        current_pcb->last_ESP = 0;

        // sets command name
        memcpy(current_pcb->com, com, strlen(com));
        
        // loads program to virtual memory
        if(read_data(dentry.inode_num, 0, (uint8_t*)(PROGRAM_START), file_system_memory[dentry.inode_num+1].byte_length) != file_system_memory[dentry.inode_num+1].byte_length)
            return -1;

        // assign a new value to the kernel stack pointer
        tss.ss0 = KERNEL_DS;
        tss.esp0 = current_pcb->EBP;

        // saves the value of esp to the pcb
        asm volatile ("movl %%esp, %0" 
            : "=r" (current_pcb->exc_ESP)
            : /* no inputs */
            : "memory" 
        );
        // saves the value of ebp to the pcb
        asm volatile ("movl %%ebp, %0" 
            : "=r" (current_pcb->exc_EBP)
            : /* no inputs */
            : "memory" 
        );
    }

    /* Initialize PIC to service PIT interrupts */
    init_pit();

    // switch to terminal 1 context
    switchContext(current_pcb->EIP);

    return 0;
}

void init_scheduling(){
    shown_terminal = 0;
    active_terminal = 0;
}

/* int32_t system_halt(int8_t status)
 * Inputs: status
 * Return Value: status
 * Function: halts the process */
int32_t system_halt(int8_t status){ // Pass status to EAX
    pcb_t* current_pcb;
    current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));

    // revert to parent PID
    pcb_t* parent = get_current_pcb(active_terminal, current_pcb->parent_PID);  // (pcb_t*)(EIGHT_MB - ((current_pcb->parent_PID + 1) * EIGHT_KB));

    // check if this is the first process
    if(current_pcb->parent_PID != -1){
        // clears the current fdt flags if the exist
        int32_t i;
        for(i=2; i<8; i++){
            if(current_pcb->file_descriptor_table[i].flags == 1)
                system_close(i);
        }

        // change paging info
        pcb_index[active_terminal] = parent->PID;
        set_program_page(pcb_index[active_terminal] +  active_terminal*4); 
        pcb_index[active_terminal]++;

        // assign a new value to the kernel stack pointer
        tss.ss0 = KERNEL_DS;
        tss.esp0 = parent->EBP;
        parent->status = status;

        if(parent->vidmap_flag && !current_pcb->vidmap_flag){
            // check if TA != TS
            if(active_terminal != shown_terminal){
                set_video_page(active_terminal+1);
            }
            // TA == TS
            else{
                set_video_page(0);
            }
        }else if(!parent->vidmap_flag && current_pcb->vidmap_flag)
            restore_video_page();

        // reduce process count
        process_count--;

        // restores execute context and returns to executer with status
        returnContext(status, current_pcb->exc_ESP, current_pcb->exc_EBP);
    }else{
        // in the event that original shell attempts to exit
        pcb_index[active_terminal] = 0;
        process_count--;
        system_execute((int8_t*)"shell");
    }

    return -1;
}

/* int32_t system_execute(const int8_t* command)
 * Inputs: command
 * Return Value: result
 * Function: executes process command */
int32_t system_execute(const int8_t* command){
    // char arg[COMMAND_LEN];
    char com[COMMAND_LEN];

    dentry_t dentry;
    pcb_t* current_pcb;

    // if(system_getargs((int8_t*)arg, COMMAND_LEN))
    //     return -1;
    if(get_keyword(command, (int8_t*)com, COMMAND_LEN))
        return -1;

    // check if keyword is exc
    char file_start[28];
    // get keyword's dentry
    if(read_dentry_by_name((uint8_t*)com, &dentry))
        return -1;

    // checks full name
    if(strncmp(dentry.file_name, com, strlen(dentry.file_name)))
        return -1;

    // get the first 28 bytes of the keyword
    if(read_data(dentry.inode_num, 0, (uint8_t*)file_start, 28) != 28)
        return -1;
    // determine if the file has magic numbers
    if(strncmp((int8_t*)file_start, (int8_t*)magic, (uint32_t)4))
        return -1;

    // find the next avail location for pcb
    if(process_count < MAX_PCB){
        current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]); // (pcb_t*)(EIGHT_MB - ((pcb_index+1) * EIGHT_KB));
        current_pcb->PID = pcb_index[active_terminal];
        store_args(command, current_pcb); //added this to call stroing args in pcb
        current_pcb->EBP = get_current_pcb_ebp(active_terminal, pcb_index[active_terminal]); // EIGHT_MB - (pcb_index * EIGHT_KB) - 4;
        set_program_page(pcb_index[active_terminal] + active_terminal*4);
        
        if(pcb_index[active_terminal] == 0)
            current_pcb->parent_PID = -1;
        else{
            current_pcb->parent_PID = get_current_pcb(active_terminal, pcb_index[active_terminal]-1)->PID;  // ((pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB)))->PID;
            if(get_current_pcb(active_terminal, pcb_index[active_terminal]-1)->vidmap_flag)
                restore_video_page();   
        }
        pcb_index[active_terminal]++;
        process_count++;
    } else
        return -1;

    // save the value of the EIP
    current_pcb->EIP = ((uint32_t*)file_start)[6];

    // add stdin and stdout for fdt
    current_pcb->file_descriptor_table[0].entries = &stdin_ops;
    current_pcb->file_descriptor_table[0].flags = 1;

    current_pcb->file_descriptor_table[1].entries = &stdout_ops;
    current_pcb->file_descriptor_table[1].flags = 1;

    // indcates no vidmap by default
    current_pcb->vidmap_flag = 0;

    // sets command name
    memcpy(current_pcb->com, com, strlen(com));
    
    // loads program to virtual memory
    if(read_data(dentry.inode_num, 0, (uint8_t*)(PROGRAM_START), file_system_memory[dentry.inode_num+1].byte_length) != file_system_memory[dentry.inode_num+1].byte_length)
        return -1;

    // assign a new value to the kernel stack pointer
    tss.ss0 = KERNEL_DS;
    tss.esp0 = current_pcb->EBP;

    // saves the value of esp to the pcb
    asm volatile ("movl %%esp, %0" 
        : "=r" (current_pcb->exc_ESP)
        : /* no inputs */
        : "memory" 
    );
    // saves the value of ebp to the pcb
    asm volatile ("movl %%ebp, %0" 
        : "=r" (current_pcb->exc_EBP)
        : /* no inputs */
        : "memory" 
    );

    // switches to user program context
    switchContext(current_pcb->EIP);

    return 0;
}

/* int32_t system_read(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: result
 * Function: reads from fd */
int32_t system_read(int32_t fd, void* buf, int32_t nbytes){
    pcb_t* current_pcb;
    current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));

    // checks if the fd is in range
    if (fd < 0 || fd > 7) {
        return -1;
    }

    // checks if stdout
    if(fd == 1)
        return -1;

    // checks for flag
    if(current_pcb->file_descriptor_table[fd].flags == 0)
        return -1;

    // find corresponding operation
    return current_pcb->file_descriptor_table[fd].entries->read(fd, buf, nbytes);
}

/* int32_t system_write(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: result
 * Function: writes from fd */
int32_t system_write(int32_t fd, const void* buf, int32_t nbytes){
    pcb_t* current_pcb;
    current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));

    // checks if the fd is in range
    if (fd < 0 || fd > 7) {
        return -1;
    }

    // checks if stdin
    if(fd == 0)
        return -1;

    // checks for flag
    if(current_pcb->file_descriptor_table[fd].flags == 0)
        return -1;
    
    // find corresponding operation
    return current_pcb->file_descriptor_table[fd].entries->write(fd, buf, nbytes);
}

/* int32_t system_open(const int8_t* filename)
 * Inputs: filename
 * Return Value: result
 * Function: open new entry in fd */
int32_t system_open(const int8_t* filename){
    dentry_t dentry;
    pcb_t* current_pcb;
    current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));
    
    // attempts to find the file
    if(read_dentry_by_name((uint8_t*)filename, &dentry))
        return -1;

    // checks full name
    if(strncmp(dentry.file_name, filename, strlen(filename)))
        return -1;

    // determines what set of operations are required
    int i;
    for (i = 2; i < 8; i++) {
        // if this file decriptor is available
        if (current_pcb->file_descriptor_table[i].flags == 0) {
            if (dentry.file_type == 0) 
            { //RTC
                set_rtc_fd(i);
                current_pcb->file_descriptor_table[i].entries = &RTC_ops;
            } 
            else if (dentry.file_type == 1) 
            { // Directory
                set_file_fd(i);
                current_pcb->file_descriptor_table[i].entries = &directory_ops;
            } 
            else if (dentry.file_type == 2) 
            { // File
                set_file_fd(i);
                current_pcb->file_descriptor_table[i].entries = &file_ops;
            }

            // set this file decriptor to be "in-use" and return i
            current_pcb->file_descriptor_table[i].flags = 1;

            // call open for this file
            if(current_pcb->file_descriptor_table[i].entries->open((uint8_t*)filename))
                return -1;

            return i;
        }
    }

    return -1;
}

/* int32_t system_close(int32_t fd)
 * Inputs: desired fd
 * Return Value: result
 * Function: close an entry in fd */
int32_t system_close(int32_t fd) {
    // determines if in range
    if (fd < 2 || fd > 7) {
        return -1;
    }
    pcb_t* current_pcb;
    current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));
    
    
    // try to close the file
    if(current_pcb->file_descriptor_table[fd].flags == 0)
        return -1;
    
    set_file_fd(fd);
    int32_t output = current_pcb->file_descriptor_table[fd].entries->close(fd);

    // if close was successfull, set the flag to "available"
    if (output == 0) {
        current_pcb->file_descriptor_table[fd].flags = 0;
    }

    return output;
}

/* int32_t system_getargs(int8_t* buf, int32_t nbytes)
 * Inputs: int8_t* buf, int32_t nbytes
 * Return Value: result
 * Function: gets args for a command */
int32_t system_getargs(int8_t* buf, int32_t nbytes){

    pcb_t* current_pcb;
    current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));
    //checking if there are no args using null terminator
    // If there are no arguments, or if the arguments and a terminal NULL (0-byte) do not fit in the buffer, simply return -1. 
    if (current_pcb->args[0] == '\0' || strlen((int8_t*)(current_pcb->args)) + 1 > nbytes)
    {
        return -1;
    }

    memcpy((char*) buf, current_pcb->args, nbytes);

    buf[nbytes - 1] = '\0';

    return 0;
}

/* int32_t system_vidmap(int8_t** screen_start)
 * Inputs: the start address of video mem
 * Return Value: result
 * Function: get video memory for a command */
int32_t system_vidmap(int8_t** screen_start){
    pcb_t* current_pcb = get_current_pcb(active_terminal, pcb_index[active_terminal]-1); // (pcb_t*)(EIGHT_MB - ((pcb_index) * EIGHT_KB));

    // checks for existing vidmap
    if(current_pcb->vidmap_flag)
        return -1;
    // checks for an address in user space
    if((uint32_t)screen_start < FOUR_MB*32 || (uint32_t)screen_start > FOUR_MB*33)
        return -1;

    // sets the address
    // check if TA != TS
    if(active_terminal != shown_terminal){
        *(screen_start) = (int8_t*)set_video_page(active_terminal+1);
    }
    // TA == TS
    else{
        *(screen_start) = (int8_t*)set_video_page(0);
    }
    current_pcb->vidmap_flag = 1;

    return 0;
}

/* int32_t get_keyword(const int8_t* command, int8_t* buf, int32_t nbytes)
 * Inputs: (const int8_t* command, int8_t* buf, int32_t nbytes)
 * Return Value: result
 * Function: gets the keyword for a command */
int32_t get_keyword(const int8_t* command, int8_t* buf, int32_t nbytes){
    // command into string
    char* output_buf = (char*)buf;
    char* raw = (char*)command;
    int i;
    
    // loop through terminal buffer and find arg
    for(i=0; i<nbytes; i++){
        // if the flag is raised and the arg is done
        if(raw[i] == ' ' || raw[i] == NULL ){
            output_buf[i] = NULL;
            break;
        }// if the flag is rasied and we are still on arg
        else{
            output_buf[i] = raw[i];
        }
    }
    return 0;
}


/*  void store_args(const int8_t* command, pcb_t* pcb)
 * Inputs: const int8_t* command, pcb_t* pcb
 * Return Value: parses the command line and stores args in pcb?
 * Function: stores args from a command */

 void store_args(const int8_t* command, pcb_t* pcb)
 {
    int command_length = strlen(command);
    //using command paramter input as user buffer

    int i = 0;

    //finding end of command
    while( i < command_length && command [i] != ' ')
    {
        i++;
    }

    //skip extra spaces to get i to start of arg
    while( i < command_length && command [i] == ' ')
    {
        i++;
    }

    if ( i < command_length)
    {
        memcpy(pcb->args, command + i, sizeof(pcb->args));
        // pcb->args[sizeof(pcb->args)] = '\0';
    }
    else
    {
        pcb->args[0] = '\0'; //no args case
    }
 }

pcb_t* get_current_pcb(uint32_t terminal, uint32_t pcb){
    return (pcb_t*)(EIGHT_MB - ((pcb+1) * EIGHT_KB) - (EIGHT_KB * 4 * terminal));
}

uint32_t get_current_pcb_ebp(uint32_t terminal, uint32_t pcb){
    return EIGHT_MB - (pcb * EIGHT_KB) - (EIGHT_KB * 4 * terminal) - 4;
}


/* void system_switch_terminals(uint32_t)
 * Inputs: uint32_t terminal; terminal number
 * Return Value: nothing
 * Function: switches terminals */
 void system_switch_terminals(uint32_t terminal)
 {
    //keyboard handler will call this based on ALT + F1 for Terminal 1 (index 0)
    
    cli();

    // edge case
    if (terminal > NUM_TERM){
        sti();
        return;
    }

    // restores one to one mapping
    set_terminal_page(0); 

    // determines where to copy current video memory to
    uint32_t copy_location;
    switch (shown_terminal){
    case 0:
        copy_location = TERM1_PAGE;
        break;
    case 1:
        copy_location = TERM2_PAGE;
        break;
    case 2:
        copy_location = TERM3_PAGE;
        break;
    default:
        sti();
        return;
    }   

    // copies current video memory to previously active terminal
    memcpy((uint8_t*)copy_location, (uint8_t*)VIDEO_MEMORY, FOUR_KB);

    // determines where to copy video memory from
    switch (terminal){
    case 0:
        copy_location = TERM1_PAGE;
        break;
    case 1:
        copy_location = TERM2_PAGE;
        break;
    case 2:
        copy_location = TERM3_PAGE;
        break;
    default:
        sti();
        return;
    } 

    // copies desired terminal video memory to video memory
    memcpy((uint8_t*)VIDEO_MEMORY, (uint8_t*)copy_location, FOUR_KB);

    shown_terminal = terminal;

    show_terminal_cursor(shown_terminal);
    set_keyboard_shown(shown_terminal);

    // check if TA != TS
    if(active_terminal != shown_terminal){
        set_terminal_page(active_terminal+1);
    }

    // gets current process
    pcb_t* current_pcb = get_current_pcb(active_terminal, pcb_index[active_keyboard_terminal]-1);

    // configures vidmap change
    if(current_pcb->vidmap_flag){
        // check if TA != TS
        if(active_terminal != shown_terminal){
            set_video_page(active_terminal+1);
        }
        // TA == TS
        else{
            set_video_page(0);
        }
    }

    sti();

 }


/* void system_schedule(uint32_t terminal)
 * Inputs: uint32_t terminal; terminal number
 * Return Value: nothing
 * Function: sets active terminal*/
void system_schedule()
{
    cli();

    // find next terminal
    uint32_t next_terminal = (active_terminal+1) % NUM_TERM;

    // gets current process
    pcb_t* current_pcb = get_current_pcb(active_terminal, pcb_index[active_keyboard_terminal]-1);

    // gets desired process
    pcb_t* new_pcb = get_current_pcb(next_terminal, pcb_index[next_terminal]-1);

    // set all dependent active terminals
    set_program_page(pcb_index[next_terminal] - 1 + next_terminal*4);

    // assign a new value to the kernel stack pointer
    tss.ss0 = KERNEL_DS;
    tss.esp0 = new_pcb->EBP;

    // sets all dependent variables
    set_screen_terminal(next_terminal);
    set_file_terminal(next_terminal);
    set_keyboard_terminal(next_terminal);
    set_rtc_terminal(next_terminal);

    // saves the value of esp to the pcb
    asm volatile ("movl %%esp, %0" 
        : "=r" (current_pcb->last_ESP)
        : /* no inputs */
        : "memory" 
    );
    // saves the value of ebp to the pcb
    asm volatile ("movl %%ebp, %0" 
        : "=r" (current_pcb->last_EBP)
        : /* no inputs */
        : "memory" 
    );

    active_terminal = next_terminal;

    // check if TA != TS
    if(active_terminal != shown_terminal){
        // handle terminal video memory mapping
        set_terminal_page(active_terminal+1);
    }
    // TA == TS
    else{
        // handle terminal video memory mapping
        set_terminal_page(0);
        // send backlogged keys
        handle_keyboard(0);
    }

    // configures vidmap change
    if(new_pcb->vidmap_flag){
        // check if TA != TS
        if(active_terminal != shown_terminal){
            set_video_page(active_terminal+1);
        }
        // TA == TS
        else{
            set_video_page(0);
        }
    }else
        restore_video_page();

    sti();

    // checks if this process has started yet
    if( new_pcb->last_EBP == 0 && new_pcb->last_ESP == 0 )
        switchContext(new_pcb->EIP);
    else // the process should be continued
        returnContext(0, new_pcb->last_ESP, new_pcb->last_EBP);

}
