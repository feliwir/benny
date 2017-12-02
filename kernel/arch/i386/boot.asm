.set MAGIC, 0xE85250D6  # multiboot2 magic
.set ARCH,	0			# i386 arch

# Declare a header as in the Multiboot Standard.
.section .multiboot
multiboot_header:
# start header
/*  magic */
.long  MAGIC
/*  ISA: i386 */
.long  ARCH
/*  Header length. */
.long  multiboot_header_end - multiboot_header
/*  checksum */
 .long  0x100000000 - (MAGIC + ARCH + (multiboot_header_end - multiboot_header))
# end header

# Information request tag
# type = 1
.short 1
# no flags
.short 0
# size = 12
.long 12
# memory map tag
.long 6
.long 0

# Null tag
.short 0
.short 0
.long  8
multiboot_header_end:

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

	# Reset EFLAGS.
	pushl $0
	popf

	# Push the pointer to the Multiboot information structure.
	pushl   %ebx

	call check_multiboot

	call setup_SSE

	# Call constructors from global objects
	call initialiseConstructors

	# Transfer control to the main kernel.
	.extern kernel_main
	call kernel_main

	# Restore stack
	add $4, %esp
	
	hlt
check_multiboot:
    cmp $0x36D76289, %eax	# this is a MB2 compatible bootloader
    jne .no_multiboot
    ret
.no_multiboot:
    movb $'0', %al
    jmp error
.size start, . - start
