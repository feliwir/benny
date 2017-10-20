.global loadGDT
.section .text
loadGDT:
    # load the gdt stuff
    sub $16, %rsp
    movq $8, 8(%rsp)
    movabsq $reloadCS, %rax
    mov %rax, (%rsp)
    lretq
reloadCS:
    movw $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret