#define ASM 1
#include "idt.h"
#include "system_calls.h"

/* MACRO to create handler linkage */
#define HANDLER_MACRO(function_name, handler, vector)    \
.globl function_name                                    ;\
.align 4                                                ;\
function_name:                                          ;\
    pushal                                              ;\
    pushfl                                              ;\
    pushl $vector                                       ;\
    call handler                                        ;\
    addl $4, %esp                                       ;\
    popfl                                               ;\
    popal                                               ;\
    iret                                                ;\



/* all defined exceptions as functions */
HANDLER_MACRO(DivisionError__, DivisionError_handler, DivisionError)

HANDLER_MACRO(Debug__, Debug_handler, Debug)

HANDLER_MACRO(NonMaskableInterrupt__, NonMaskableInterrupt_handler, NonMaskableInterrupt)

HANDLER_MACRO(Breakpoint__, Breakpoint_handler, Breakpoint)

HANDLER_MACRO(Overflow__, Overflow_handler, Overflow)

HANDLER_MACRO(BoundRangeExceeded__, BoundRangeExceeded_handler, BoundRangeExceeded)

HANDLER_MACRO(InvalidOpcode__, InvalidOpcode_handler, InvalidOpcode)

HANDLER_MACRO(DeviceNotAvailable__, DeviceNotAvailable_handler, DeviceNotAvailable)

HANDLER_MACRO(DoubleFault__, DoubleFault_handler, DoubleFault)

HANDLER_MACRO(CoprocessorSegmentOverrun__, CoprocessorSegmentOverrun_handler, CoprocessorSegmentOverrun)

HANDLER_MACRO(InvalidTSS__, InvalidTSS_handler, InvalidTSS)

HANDLER_MACRO(SegmentNotPresent__, SegmentNotPresent_handler, SegmentNotPresent)

HANDLER_MACRO(StackSegmentFault__, StackSegmentFault_handler, StackSegmentFault)

HANDLER_MACRO(GeneralProtectionFault__, GeneralProtectionFault_handler, GeneralProtectionFault)

HANDLER_MACRO(PageFault__, PageFault_handler, PageFault)

HANDLER_MACRO(Reserved__, Reserved_handler, Reserved)

HANDLER_MACRO(FloatingPointException__, FloatingPointException_handler, FloatingPointException)

HANDLER_MACRO(AlignmentCheck__, AlignmentCheck_handler, AlignmentCheck)

HANDLER_MACRO(MachineCheck__, MachineCheck_handler, MachineCheck)

HANDLER_MACRO(SIMDFloatingPointException__, SIMDFloatingPointException_handler, SIMDFloatingPointException)

HANDLER_MACRO(VirtualizationException__, VirtualizationException_handler, VirtualizationException)

HANDLER_MACRO(Keyboard__, handle_keyboard, Keyboard)

HANDLER_MACRO(RTC__, handle_rtc, RTC)

HANDLER_MACRO(PIT__, handle_pit, PIT)


// HANDLER_MACRO(SystemCall__, handle_system_call, SystemCall)
.text
.globl SystemCall__
SystemCall__:
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushfl

    cmpl $8, %eax      // checks if eax > 8
    ja not_valid
    cmpl $0, %eax       // checks if eax == 0
    je not_valid

    jmp *jump_table(,%eax,4)        // jumps to corresponing system call linkage

done:                   // post linkage return
    popfl
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    iret

not_valid:              // return for not valid system calls
    popfl
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    movl $-1, %eax
    iret

halt_link:
    pushl %ebx          // pushs args
    call system_halt
    addl $4, %esp
    jmp done

execute_link:
    pushl %ebx          // pushs args
    call system_execute
    addl $4, %esp
    jmp done

read_link:
    pushl %edx          // pushs args
    pushl %ecx
    pushl %ebx
    call system_read
    addl $12, %esp
    jmp done

write_link:
    pushl %edx          // pushs args
    pushl %ecx
    pushl %ebx
    call system_write
    addl $12, %esp
    jmp done

open_link:
    pushl %ebx          // pushs args
    call system_open
    addl $4, %esp
    jmp done

close_link:
    pushl %ebx          // pushs args
    call system_close
    addl $4, %esp
    jmp done

getargs_link:
    pushl %ecx          // pushs args
    pushl %ebx
    call system_getargs
    addl $8, %esp
    jmp done

vidmap_link:
    pushl %ebx          // pushs args
    call system_vidmap
    addl $4, %esp
    jmp done

jump_table:             // jump table for linkages
    .long done
    .long halt_link
    .long execute_link
    .long read_link
    .long write_link
    .long open_link
    .long close_link
    .long getargs_link
    .long vidmap_link
