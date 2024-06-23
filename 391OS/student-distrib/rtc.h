#ifndef _RTC_H
#define _RTC_H

#include "lib.h"
#include "i8259.h"

#define FD_SIZE 8
#define DEFAULT_FREQ 512
#define NUM_TERM 3

/* Function to initialize PIC to handle RTC interrupts */
void init_rtc();

void set_rtc_fd(uint32_t fd);

void set_rtc_terminal(uint32_t terminal);

/*RTC open() initializes RTC frequency to 2HZ, return 0*/
int32_t open_rtc(const uint8_t* filename);

/*RTC close() probably does nothing, unless you virtualize RTC, return 0*/
int32_t close_rtc(int32_t fd);

/*RTC read() should block (wait) until the next interrupt, return 0*/
int32_t read_rtc(int32_t fd, void* buf, int32_t nbytes);

/*RTC write() must be able to change frequency, return 0 or -1*/
int32_t write_rtc(int32_t fd, const void* buf, int32_t nbytes);

typedef struct rtc_data{
    uint32_t freq;
    uint32_t count;
} rtc_data_t;

volatile uint32_t terminal_count[NUM_TERM];

#endif /* _RTC_H */
