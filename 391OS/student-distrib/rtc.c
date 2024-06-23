#include "rtc.h"

volatile int rtc_interrupt_occured = 0;

uint32_t r_index[NUM_TERM];
rtc_data_t r_rtc[NUM_TERM][FD_SIZE];
uint32_t active_rtc_terminal = 0;

/* void init_rtc()
 * Inputs: none
 * Return Value: none
 * Function: sets default value and initializes rtc */
void init_rtc() {

    // Keep the RTC from being left in an undefined state.

    // Enable Interrupts
    cli();      

    // 0x70 = CMOS port, 0x71 = RTC port
    outb(0x8A, 0x70);

    // Enable Interrupts
    cli();

    // B status register data goes to CMOS port
    outb(0x8B, 0x70);

    // Save previous state
    char prev = inb(0x71);

    // Update CMOS port again
    outb(0x8B, 0x70);

    // Or logic to make 7th bit 1
    outb(prev | 0x40, 0x71);

    // RTC interrupts are serviced by irq8 (irq0 on slave PIC)
    enable_irq(8);

    // initializes default frequency
    cli();

    outb(0x8A, 0x70);		    // 0x70 = CMOS port, 0x70 = NMI

    char rate = 0x07;           // 512 Hz rate

    prev = inb(0x71);	    // get initial value of register A

    outb(0x8A, 0x70);		    // reset index to A

    outb((prev & 0xF0) | rate, 0x71); 
    
    // sti();
}    

/* void set_rtc_fd(uint32_t fd)
 * Inputs: fd
 * Return Value: none
 * Function: sets the current operation fd */
void set_rtc_fd(uint32_t fd){
    r_index[active_rtc_terminal] = fd;
}

/* void set_rtc_terminal(int32_t terminal)
 * Inputs: int32_t terminal
 * Return Value: none
 * Function: sets the current terminal */
void set_rtc_terminal(uint32_t terminal){
    active_rtc_terminal = terminal;
}

/* int32_t open_rtc(const uint8_t* filename)
 * Inputs: filename
 * Return Value: result
 * Function: creates an entry in the the rtc */
int32_t open_rtc(const uint8_t* filename){

    // sets default freq and count
    r_rtc[active_rtc_terminal][r_index[active_rtc_terminal]].freq = 2;

    r_rtc[active_rtc_terminal][r_index[active_rtc_terminal]].count = 0;

    return 0;

}

/* int32_t close_rtc(int32_t fd)
 * Inputs: fd
 * Return Value: result
 * Function: removes an entry in the the rtc */
int32_t close_rtc(int32_t fd){
    return 0;
}

/* int32_t read_rtc(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: result
 * Function: waits for a specified time */
int32_t read_rtc(int32_t fd, void* buf, int32_t nbytes){
    // int i;
    terminal_count[active_rtc_terminal] = 0;
    // rtc_interrupt_occured = 0;

    while(1) {
        // checks if we have gotten the desired number of interrupts
        if( terminal_count[active_rtc_terminal] >= DEFAULT_FREQ / r_rtc[active_rtc_terminal][fd].freq )   
            break;
    } 

    return 0; 
}

/* int32_t write_rtc(int32_t fd, const void* buf, int32_t nbytes)
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: result
 * Function: writes an entry's frequency */
int32_t write_rtc(int32_t fd, const void* buf, int32_t nbytes){

    uint32_t freq = *((uint32_t*)buf);

    // checks frequency bounds
    if( freq < 2 || freq > DEFAULT_FREQ )
        return -1;

    // checks if the freq is even
    if(freq & (freq - 1)){
        return -1;
    }

    // assigns the freq to fd
    r_rtc[active_rtc_terminal][fd].freq = freq;

    return 0;
}


