.global loadGDT
.section .text
loadGDT:
    # pop the return adress from the stack
    pop %rax
    push $0x10
    push %rsp

    pushfq
    # call reloadCS
    push $0x08

    # push our return adress
    push %rax
    iretq
reloadCS:
    movw $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret