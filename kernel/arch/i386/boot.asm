#include <multiboot.h>

# Declare constants used for creating a multiboot header.
.set MAGIC,   	0xE85250D6  # 'magic number' lets bootloader find the header
.set ARCH,		0			# 0 x86, 4 MIPS

# Declare a header as in the Multiboot Standard.
.section .multiboot
header_start:
# start header
.long MAGIC
.long ARCH
.long header_end-header_start
.long 0x100000000 - (MAGIC + 0 + (header_end - header_start))
# end header
.long 0
.long 8
header_end:

# Reserve a stack for the initial thread.
.section .bss, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
	
# The kernel entry point.
.section .text
.global start
.type start, @function
start:
	mov $stack_top, %esp

	call check_multiboot

	call setup_SSE

	# Call constructors from global objects
	call initialiseConstructors

	# Transfer control to the main kernel.
	.extern kernel_main
	jmp kernel_main
check_multiboot:
    cmp $0x2BADB002, %eax
    jne .no_multiboot
    ret
.no_multiboot:
    movb $'0', %al
    jmp error
.size _start, . - _start
