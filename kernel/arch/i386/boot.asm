#define ASM_FILE        1
#include <multiboot.h>

# Declare a header as in the Multiboot Standard.
.section .multiboot
multiboot_header:
# start header
/*  magic */
.long   MULTIBOOT2_HEADER_MAGIC
/*  ISA: i386 */
.long   MULTIBOOT_ARCHITECTURE_I386
/*  Header length. */
.long   multiboot_header_end - multiboot_header
/*  checksum */
.long  - (MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
# end header
.short MULTIBOOT_HEADER_TAG_END
.short 0
.long 8
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
    cmp $0x36D76289, %eax
    jne .no_multiboot
    ret
.no_multiboot:
    movb $'0', %al
    jmp error
.size start, . - start
