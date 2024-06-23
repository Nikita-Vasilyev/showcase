#include "idt.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "terminal.h"
#include "rtc.h"
#include "system_calls.h"

static char* execption_str[] = {"DivisionError", "Debug", "NonMaskableInterrupt", "Breakpoint", 
                "Overflow", "BoundRangeExceeded", "InvalidOpcode", "DeviceNotAvailable", "DoubleFault", 
                "CoprocessorSegmentOverrun", "InvalidTSS", "SegmentNotPresent", "StackSegmentFault", 
                "GeneralProtectionFault", "PageFault", "Reserved", "FloatingPointException", "AlignmentCheck",
                "MachineCheck", "SIMDFloatingPointException", "VirtualizationException"};

// define all asm linkage functions
extern void DivisionError__();

extern void Debug__();

extern void NonMaskableInterrupt__();

extern void Breakpoint__();

extern void Overflow__();

extern void BoundRangeExceeded__();

extern void InvalidOpcode__();

extern void DeviceNotAvailable__();

extern void DoubleFault__();

extern void CoprocessorSegmentOverrun__();

extern void InvalidTSS__();

extern void SegmentNotPresent__();

extern void StackSegmentFault__();

extern void GeneralProtectionFault__();

extern void PageFault__();

extern void Reserved__();

extern void FloatingPointException__();

extern void AlignmentCheck__();

extern void MachineCheck__();

extern void SIMDFloatingPointException__();

extern void VirtualizationException__();

extern void Keyboard__();

extern void RTC__();

extern void PIT__();

extern void SystemCall__();

/* Function invoked in keyboard interrupt handler, self-explanatory */
char get_ascii_from_scan_code(uint8_t scan_code);

// Define a struct for the lookup table entry
typedef struct {
    uint8_t scan_code;
    char ascii_value;       // default
    char shift_ascii_value; // shift
    char caps_ascii_value; // caps-lock
} ScanCodeLookup;

static uint8_t shift_flag = 0;
static uint8_t ctrl_flag = 0;
static uint8_t caps_flag = 0;
static uint8_t alt_flag = 0;

// Initialize the lookup table with the first few letters and numbers
ScanCodeLookup lookup_table[] = {
    {0x1E, 'a', 'A', 'A'}, // Scan code for 'A' key
    {0x30, 'b', 'B', 'B'}, // Scan code for 'B' key
    {0x2E, 'c', 'C', 'C'}, // Scan code for 'C' key
    {0x20, 'd', 'D', 'D'}, // Scan code for 'D' key
    {0x12, 'e', 'E', 'E'}, // Scan code for 'E' key
    {0x21, 'f', 'F', 'F'}, // Scan code for 'F' key
    {0x22, 'g', 'G', 'G'}, // Scan code for 'G' key
    {0x23, 'h', 'H', 'H'}, // Scan code for 'H' key
    {0x17, 'i', 'I', 'I'}, // Scan code for 'I' key
    {0x24, 'j', 'J', 'J'}, // Scan code for 'J' key
    {0x25, 'k', 'K', 'K'}, // Scan code for 'K' key
    {0x26, 'l', 'L', 'L'}, // Scan code for 'L' key
    {0x32, 'm', 'M', 'M'}, // Scan code for 'M' key
    {0x31, 'n', 'N', 'N'}, // Scan code for 'N' key
    {0x18, 'o', 'O', 'O'}, // Scan code for 'O' key
    {0x19, 'p', 'P', 'P'}, // Scan code for 'P' key
    {0x10, 'q', 'Q', 'Q'}, // Scan code for 'Q' key
    {0x13, 'r', 'R', 'R'}, // Scan code for 'R' key
    {0x1F, 's', 'S', 'S'}, // Scan code for 'S' key
    {0x14, 't', 'T', 'T'}, // Scan code for 'T' key
    {0x16, 'u', 'U', 'U'}, // Scan code for 'U' key
    {0x2F, 'v', 'V', 'V'}, // Scan code for 'V' key
    {0x11, 'w', 'W', 'W'}, // Scan code for 'W' key
    {0x2D, 'x', 'X', 'X'}, // Scan code for 'X' key
    {0x15, 'y', 'Y', 'Y'}, // Scan code for 'Y' key
    {0x2C, 'z', 'Z', 'Z'}, // Scan code for 'Z' key
    {0x02, '1', '!', '1'}, // Scan code for '1' key
    {0x03, '2', '@', '2'}, // Scan code for '2' key
    {0x04, '3', '#', '3'}, // Scan code for '3' key
    {0x05, '4', '$', '4'}, // Scan code for '4' key
    {0x06, '5', '%', '5'}, // Scan code for '5' key
    {0x07, '6', '^', '6'}, // Scan code for '6' key
    {0x08, '7', '&', '7'}, // Scan code for '7' key
    {0x09, '8', '*', '8'}, // Scan code for '8' key
    {0x0A, '9', '(', '9'}, // Scan code for '9' key
    {0x0B, '0', ')', '0'},  // Scan code for '0' key
    {0x39, ' ', ' ', ' '}, // Scan code for space key
    {0x0C, '-', '_', '-'},
    {0x0D, '=', '+', '='},
    {0x1A, '[', '{', '['},
    {0x1B, ']', '}', ']'},
    {0x27, ';', ':', ';'},
    {0x28, '\'', '\"', '\''},
    {0x29, '`', '~', '`'},
    {0x2B, '\\', '|', '\\'},
    {0x33, ',', '<', ','},
    {0x34, '.', '>', '.'},
    {0x35, '/', '?', '/'}
};

// Size of the lookup table
const int lookup_table_size = 48;

// all handler functions
void DivisionError_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[0]);
    system_halt(1);
}

void Debug_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[1]);
    system_halt(1);
}

void NonMaskableInterrupt_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[2]);
    system_halt(1);
}

void Breakpoint_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[3]);
    system_halt(1);
}

void Overflow_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[4]);
    system_halt(1);
}

void BoundRangeExceeded_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[5]);
    system_halt(1);
}

void InvalidOpcode_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[6]);
    system_halt(1);
}

void DeviceNotAvailable_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[7]);
    system_halt(1);
}

void DoubleFault_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[8]);
    system_halt(1);
}

void CoprocessorSegmentOverrun_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[9]);
    system_halt(1);
}

void InvalidTSS_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[10]);
    system_halt(1);
}

void SegmentNotPresent_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[11]);
    system_halt(1);
}

void StackSegmentFault_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[12]);
    system_halt(1);
}

void GeneralProtectionFault_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[13]);
    system_halt(1);
}

void PageFault_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[14]);
    system_halt(1);
}

void Reserved_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[15]);
    system_halt(1);
}

void FloatingPointException_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[16]);
    system_halt(1);
}

void AlignmentCheck_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[17]);
    system_halt(1);
}

void MachineCheck_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[18]);
    system_halt(1);
}


void SIMDFloatingPointException_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[19]);
    system_halt(1);
}

void VirtualizationException_handler(uint32_t vector) {
    printf("%s Exception\n", execption_str[20]);
    system_halt(1);
}

//declare keyboard buffer and index
static uint8_t key_index[NUM_TERM];

int8_t backlogKeys[KEY_BUF_SIZE];
uint32_t backlog_index = 0;

// clears the current buffer
void clear_keyboard_buffer(){
    key_index[active_keyboard_terminal] = 0;
    memset(KeyboardBuffer[active_keyboard_terminal], 0, KEY_BUF_SIZE);
}

// clears the current buffer
void clear_backlog_buffer(){
    backlog_index = 0;
    memset(backlogKeys, NULL, KEY_BUF_SIZE);
}

void handle_keyboard(uint32_t vector){
    int i;
    cli();
    // sends the backlogged keys
    if(vector == 0){
        for(i=0; i<KEY_BUF_SIZE; i++){
            // checks for end of string
            if(backlogKeys[i] == NULL)
                break;
            // checks for backspace
            else if(backlogKeys[i] == -1){
                if(key_index[active_keyboard_terminal] > 0 && rmc() == 0){
                    KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = NULL;
                    key_index[active_keyboard_terminal]--;
                    KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = NULL;
                }
            }
            else if(backlogKeys[i] == -2){
                clear_screen();
            }
            // print keys
            else if(key_index[active_keyboard_terminal] < KEY_BUF_SIZE){
                putc(backlogKeys[i]);
                KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = backlogKeys[i];
                key_index[active_keyboard_terminal]++;
            }
        }
        clear_backlog_buffer();
        sti();
        return;
    }
    
    // 0x60 keyboard port
    int result = inb(0x60);

    // only write when TA == TS. otherwise backlog
    uint32_t write = (shown_keyboard_terminal == active_keyboard_terminal);

    if (!new_line[shown_keyboard_terminal]){
        // checks for the ctrl flag for two key inputs
        if(result == 0x1D)
            ctrl_flag++;
        else if(result == 0x9D)
            ctrl_flag--;

        // check for shift key
        else if(result == 0x2A || result == 0x36)
            shift_flag++;
        else if(result == 0xAA || result == 0xB6)
            shift_flag--;

        // check for caps lock state
        else if(result == 0x3A)
            caps_flag = ~caps_flag;

        // terminal 1
        else if(alt_flag && result == 0x3B){
            send_eoi(1);
            system_switch_terminals(0); // system switch will restore interrupts after processing new terminal
            return;
        }// terminal 2
        else if(alt_flag && result == 0x3C){
            send_eoi(1);
            system_switch_terminals(1); // system switch will restore interrupts after processing new terminal
            return;
        }// terminal 3
        else if(alt_flag && result == 0x3D){
            send_eoi(1);
            system_switch_terminals(2); // system switch will restore interrupts after processing new terminal
            return;
        }
        // look for all function keys
        else if((result <= 0x46 && result >= 0x44) || result == 0x57 || result == 0x58)
            ;

        // ignore all unneeded keys
        else if((result >= 0x45 && result <= 0x53) || result == 0x37)
            ;

        // handle backspace
        else if(result == 0x0E){
            if(write && key_index[active_keyboard_terminal] > 0){
                if(rmc() == 0){
                    KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = NULL;
                    key_index[active_keyboard_terminal]--;
                    KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = NULL;
                }
            } 
            // adds remove flag
            else if(!write){
                backlogKeys[backlog_index] = -1;
                backlog_index++;
            }
        }

        // handle left alt
        else if(result == 0x38)
            alt_flag++;
        else if(result == 0xB8)
            alt_flag--;

        // handle tab
        else if(result == 0x0F){
            // attempts to print 4 spaces
            for(i=0; i<4; i++){
                int ascii_val = 0x20;
                // checks if buffer is maxed out
                if(key_index[active_keyboard_terminal] < MAX_KEY_BUF && write){
                    putc(ascii_val);
                    //send to keyboard buffer also
                    KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = ascii_val;
                    key_index[active_keyboard_terminal]++;
                }else if(backlog_index < MAX_KEY_BUF){
                    backlogKeys[backlog_index] = ascii_val;
                    backlog_index++;
                }
            }
        }

        // handle ctrl + l/L
        else if(result == 0x26 && ctrl_flag){
            if(write)
                clear_screen();
            else{
                backlogKeys[backlog_index] = -2;
                backlog_index++;
            }
        }

        // handle enter
        else if(result == 0x1C){
            // newline character
            if(write)
                putc(0x0a);
            else{
                backlogKeys[backlog_index] = 0x0a;
                backlog_index++;
            }
            new_line[shown_keyboard_terminal] = 1;
        }

        // consider all chars
        else if(result <= 0x39 && result >= 0x02){
            // checks if buffer is maxed out
            char ascii_val = get_ascii_from_scan_code(result);
            if(key_index[active_keyboard_terminal] < MAX_KEY_BUF && write){
                putc(ascii_val);
                //send to keyboard buffer also
                KeyboardBuffer[active_keyboard_terminal][key_index[active_keyboard_terminal]] = ascii_val;
                key_index[active_keyboard_terminal]++;
            }
            else if(backlog_index < MAX_KEY_BUF){
                backlogKeys[backlog_index] = ascii_val;
                backlog_index++;
            }
        }
    }
    sti();
    send_eoi(1);
}

void handle_rtc(uint32_t vector){
    int i;

    cli();

    // 0x70 = CMOS port, 0x71 = RTC port
    outb(0x8C, 0x70);

    inb(0x71);

    // increment all the terminal counts
    for(i=0; i<NUM_TERM; i++)
        terminal_count[i]++;

    send_eoi(8);

    sti();
}

void handle_pit(uint32_t vector){
    cli();
    send_eoi(0);
    system_schedule(); // system schedule will restore interrupts after processing new terminal
}

void populateIDT() {

    // loop through expection attrubutes
    int i;
    for (i = 0; i < 20; i++) {
        idt[i].size = 1;
        if(i != Reserved){
            // set params
            idt[i].present = 1;
            idt[i].size = 1;
            idt[i].dpl = 0;
            idt[i].seg_selector = KERNEL_CS;

            // trap gate config based on OSDev Doc
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 1;
        }
    }

    // set all entries in IDT
    SET_IDT_ENTRY(idt[DivisionError], DivisionError__);

    SET_IDT_ENTRY(idt[Debug], Debug__);

    SET_IDT_ENTRY(idt[NonMaskableInterrupt], NonMaskableInterrupt__);

    SET_IDT_ENTRY(idt[Breakpoint], Breakpoint__);

    SET_IDT_ENTRY(idt[Overflow], Overflow__);

    SET_IDT_ENTRY(idt[BoundRangeExceeded], BoundRangeExceeded__);

    SET_IDT_ENTRY(idt[InvalidOpcode], InvalidOpcode__);

    SET_IDT_ENTRY(idt[DeviceNotAvailable], DeviceNotAvailable__);

    SET_IDT_ENTRY(idt[DoubleFault], DoubleFault__);

    SET_IDT_ENTRY(idt[CoprocessorSegmentOverrun], CoprocessorSegmentOverrun__);

    SET_IDT_ENTRY(idt[InvalidTSS], InvalidTSS__);

    SET_IDT_ENTRY(idt[SegmentNotPresent], SegmentNotPresent__);

    SET_IDT_ENTRY(idt[StackSegmentFault], StackSegmentFault__);

    SET_IDT_ENTRY(idt[GeneralProtectionFault], GeneralProtectionFault__);

    SET_IDT_ENTRY(idt[PageFault], PageFault__);

    SET_IDT_ENTRY(idt[Reserved], Reserved__);

    SET_IDT_ENTRY(idt[FloatingPointException], FloatingPointException__);

    SET_IDT_ENTRY(idt[AlignmentCheck], AlignmentCheck__);

    SET_IDT_ENTRY(idt[MachineCheck], MachineCheck__);

    SET_IDT_ENTRY(idt[SIMDFloatingPointException], SIMDFloatingPointException__);

    SET_IDT_ENTRY(idt[VirtualizationException], VirtualizationException__);

    // configure system call
    idt[SystemCall].present = 1;
    idt[SystemCall].size = 1;
    idt[SystemCall].dpl = 3;
    idt[SystemCall].seg_selector = KERNEL_CS;

    // trap gate config
    idt[SystemCall].reserved1 = 1;
    idt[SystemCall].reserved2 = 1;
    idt[SystemCall].reserved3 = 1;

    // create function
    SET_IDT_ENTRY(idt[SystemCall], SystemCall__);

    // configure keyboard call
    idt[Keyboard].present = 1;
    idt[Keyboard].size = 1;
    idt[Keyboard].dpl = 0;
    idt[Keyboard].seg_selector = KERNEL_CS;

    // interrupt gate config
    idt[Keyboard].reserved1 = 1;
    idt[Keyboard].reserved2 = 1;
    idt[Keyboard].reserved3 = 0;

    // create function
    SET_IDT_ENTRY(idt[Keyboard], Keyboard__);

    // configure RTC call
    idt[RTC].present = 1;
    idt[RTC].size = 1;
    idt[RTC].dpl = 0;
    idt[RTC].seg_selector = KERNEL_CS;

    // interrupt gate config
    idt[RTC].reserved1 = 1;
    idt[RTC].reserved2 = 1;
    idt[RTC].reserved3 = 0;

    // create function
    SET_IDT_ENTRY(idt[RTC], RTC__);

    // configure PIT call
    idt[PIT].present = 1;
    idt[PIT].size = 1;
    idt[PIT].dpl = 0;
    idt[PIT].seg_selector = KERNEL_CS;

    // interrupt gate config
    idt[PIT].reserved1 = 1;
    idt[PIT].reserved2 = 1;
    idt[PIT].reserved3 = 0;

    // create function
    SET_IDT_ENTRY(idt[PIT], PIT__);

    lidt(idt_desc_ptr);
}

// Function to find the ASCII value given a scan code
char get_ascii_from_scan_code(uint8_t scan_code) {
    int i;
    for (i = 0; i < lookup_table_size; i++) {
        if (lookup_table[i].scan_code == scan_code) 
        {
            //checking whether shift and caps are pressed to return the approproate value
            if (shift_flag && !caps_flag)
            {
                return lookup_table[i].shift_ascii_value;
            }

            else if(shift_flag && caps_flag){
                if(lookup_table[i].ascii_value == lookup_table[i].caps_ascii_value)
                    return lookup_table[i].shift_ascii_value;
                else
                    return lookup_table[i].ascii_value;
            }
            //if caps is presssed return the caps value of the key pressed
            else if(caps_flag){
                return lookup_table[i].caps_ascii_value;
            }
            //return the normal value
            else
            {
                return lookup_table[i].ascii_value; // Return the ASCII value if found
            }
            
        }
    }
    return 0; // Return 0 if not found
}
