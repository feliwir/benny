# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
	
# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp

	call check_multiboot

	# Call constructors from global objects
	call initialiseConstructors

	# Transfer control to the main kernel.
	.extern kernel_main
	jmp kernel_main
error:
    movl $0x4f524f45, 0xb8000
	movl $0x4f3a4f52, 0xb8004
	movl $0x4f204f20, 0xb8008	
	movb %al		, 0xb800a
    hlt
check_multiboot:
    cmp $0x2BADB002, %eax
    jne .no_multiboot
    ret
.no_multiboot:
    movb $'0', %al
    jmp error
.size _start, . - _start
