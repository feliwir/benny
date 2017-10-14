.set MAGIC, 0xE85250D6  # multiboot2 magic
.set ARCH,	0			# i386 arch

# Declare a header as in the Multiboot Standard.
.section .multiboot
multiboot_header:
# start header
/*  magic */
.long  MAGIC
/*  ISA: i386 */
.long   ARCH
/*  Header length. */
.long   multiboot_header_end - multiboot_header
/*  checksum */
 .long  -(MAGIC + ARCH + (multiboot_header_end - multiboot_header))
# end header
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

	call check_multiboot

	call setup_SSE

	# Call constructors from global objects
	call initialiseConstructors

	# Transfer control to the main kernel.
	.extern kernel_main
	jmp kernel_main
check_multiboot:
    cmp $0x36D76289, %eax	# this is a MB2 compatible bootloader
    jne .no_multiboot
    ret
.no_multiboot:
    movb $'0', %al
    jmp error
.size start, . - start
