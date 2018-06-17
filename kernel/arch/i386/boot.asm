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
.size start, . - start
