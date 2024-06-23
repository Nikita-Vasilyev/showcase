#define ASM 1

#include "system_calls.h"
#include "x86_desc.h"

.text
.globl switchContext
switchContext:
    popl %eax
    movl (%esp), %eax           // moves new eip to eax
    pushl $USER_DS              // SS
    pushl $(USER_STACK - 4)     // ESP
    pushfl                      // EFLAGS
    pushl $USER_CS              // CS
    pushl %eax                  // EIP
    iret                        // switch context   

.text
.globl returnContext
returnContext:
    movl 4(%esp), %eax          
    movl 12(%esp), %ebp
    movl 8(%esp), %esp          // changes to execute context
    leave
    ret
