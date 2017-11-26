.global loadGDT
.section .text
loadGDT:
    # Save the current stack pointer on the stack to push later
    popq %rax
    pushq %rsp

    # push ss
    pushq $0x10

    # push rsp
    pushq 8(%rsp)


    pushfq
    # push cs
    pushq $0x08
    # push eip back on
    pushq %rax

    call reloadCS

    iretq

reloadCS:
    pushw %ax

    movw $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    popw %ax
    ret