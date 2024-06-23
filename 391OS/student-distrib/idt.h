#ifndef IDT_H
#define IDT_H

/* first 20 exceptions, keyboard int, RTC int */
#define DivisionError 0
#define Debug 1
#define NonMaskableInterrupt 2
#define Breakpoint 3
#define Overflow 4
#define BoundRangeExceeded 5
#define InvalidOpcode 6
#define DeviceNotAvailable 7
#define DoubleFault 8
#define CoprocessorSegmentOverrun	9
#define InvalidTSS	10
#define SegmentNotPresent	11
#define StackSegmentFault	12
#define GeneralProtectionFault 13
#define PageFault 14
#define Reserved 15
#define FloatingPointException 16
#define AlignmentCheck	17
#define MachineCheck 18
#define SIMDFloatingPointException 19
#define VirtualizationException 20

/* interrupts */
#define PIT 0x20
#define Keyboard 0x21
#define RTC 0x28

/* define 0x80 system calls (handler returns 0) */
#define SystemCall 0x80

/* only define function when included in c file */
#ifndef ASM

#include "types.h"

/* Function to populate IDT */
void populateIDT();

void clear_keyboard_buffer();

void handle_keyboard(uint32_t vector);

#endif

#endif
