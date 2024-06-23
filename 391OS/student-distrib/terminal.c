#include "terminal.h"
#include "idt.h"

// static char terminal_buffer[128];
// active_keyboard_terminal = 0;

/* void set_keyboard_terminal(int32_t terminal)
 * Inputs: int32_t terminal
 * Return Value: none
 * Function: sets the current terminal */
void set_keyboard_terminal(uint32_t terminal){
    active_keyboard_terminal = terminal;
}

/* void set_keyboard_shown(int32_t terminal)
 * Inputs: int32_t terminal
 * Return Value: none
 * Function: sets the current terminal */
void set_keyboard_shown(uint32_t terminal){
    shown_keyboard_terminal = terminal;
}

/* int8_t terminal_open();
 * Inputs: void
 * Return Value: returns 0 upon success
 * Function: inits and opens terminal */
int32_t terminal_open(const uint8_t* filename){
    // init_terminal();
    return 0;
}

/* int8_t terminal_close();
 * Inputs: void
 * Return Value: returns 0 upon success
 * Function: closes terminal */
int32_t terminal_close(int32_t fd){
    return 0;
}

/* int8_t terminal_read(char keyboard_buffer[KEY_BUF_SIZE]);
 * Inputs: keyboard_buffer of size 128
 * Return Value: returns number of bytes read or -1 if buffer is invalid
 * Function: reads from terminal buffer */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    int i;
    char* output_buf = (char*)buf;

    if( output_buf == NULL )
        return -1;

    new_line[active_keyboard_terminal] = 0;

    while (!new_line[active_keyboard_terminal]) {}

    for(i=0; i<nbytes; i++){
        // checks if the buffer has ended
        if(KeyboardBuffer[active_keyboard_terminal][i] != NULL){
            output_buf[i] = KeyboardBuffer[active_keyboard_terminal][i];
        }
        else
            break;
    }

    clear_keyboard_buffer();
    new_line[active_keyboard_terminal] = 0;
    return i;
}

/* int8_t terminal_write();
 * Inputs: void
 * Return Value: returns number of bytes written
 * Function: writes to terminal buffer */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    int i = 0;
    char* print_buf = (char*)buf;

    if(print_buf == NULL)
        return -1;

    for(i=0; i<nbytes; i++){
        if (print_buf[i] != '\0') {
            putc(print_buf[i]);
        } else {
            continue;
        }
    }
    // returns the size of the buffer
    return i;
}
