.code32
.global setup_SSE
.section .text
setup_SSE:
    # check for SSE
    movl $0x1, %eax
    cpuid
    test $(1<<25), %edx
    jz .no_SSE

    # enable SSE
    movl %cr0, %eax
    and $0xFFFB, %ax    # clear coprocessor emulation CR0.EM
    or $0x2, %ax        # set coprocessor monitoring  CR0.MP
    mov %eax, %cr0
    mov %cr4, %eax
    or $(3 << 9),%ax    # set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    mov %eax, %cr4

    ret
.no_SSE:
    movb $'0', %al
    jmp error