#ifndef TERM_H
#define TERM_H

#include "lib.h"

#define KEY_BUF_SIZE 128

#define MAX_KEY_BUF 127

int32_t terminal_open(const uint8_t* filename);

int32_t terminal_close(int32_t fd);

int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);

int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);

void set_keyboard_terminal(uint32_t terminal);

void set_keyboard_shown(uint32_t terminal);

volatile int32_t new_line[NUM_TERM];
char KeyboardBuffer [NUM_TERM][MAX_KEY_BUF+1];
uint32_t active_keyboard_terminal;
uint32_t shown_keyboard_terminal;

#endif /* TERM_H */
