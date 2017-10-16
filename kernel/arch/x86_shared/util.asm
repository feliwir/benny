.code32
.global loadGDT
.section .text
loadGDT:
    # load the gdt stuff
    jmp  $0x08, $reloadCS
reloadCS:
    movw $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret