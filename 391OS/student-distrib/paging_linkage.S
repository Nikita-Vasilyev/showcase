#define ASM 1
#include "paging.h"


.text
.globl enablePaging
enablePaging:
    pushl %ebp
    mov %esp, %ebp
    mov %cr4, %eax
    or $0x00000010, %eax
    mov %eax, %cr4
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    mov %ebp, %esp
    popl %ebp
    ret

.text
.globl loadPageDirectory
loadPageDirectory:
    pushl %ebp
    mov %esp, %ebp
    mov 8(%esp), %eax
    mov %eax, %cr3
    mov %ebp, %esp
    popl %ebp
    ret

.globl flushTLB
flushTLB:
    pushl %ebp
    mov %esp, %ebp
	movl	%cr3,%eax
	movl	%eax,%cr3
    leave
    ret
