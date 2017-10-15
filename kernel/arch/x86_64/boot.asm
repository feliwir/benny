.code32
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
.short 0
.short 0
.long  8
multiboot_header_end:

# Reserve a stack for the initial thread.
.section .bss, "aw", @nobits
p4_table:
	.align 4096    # MDP: Page table need 4096 byte alignent
	.skip 4096
p3_table:
	.skip 4096
p2_table:
	.skip 4096
stack_bottom:
	.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp           # MDP: Missing $

	# check for cpuid
	call check_multiboot
	call check_cpuid
	# Setup SSE
	call setup_SSE
	call check_long_mode
	call set_up_page_tables
	call enable_paging
	# load our new 64 bit GDT
	lgdt init_gdtr
	jmp  $0x18, $longmode

.code64
longmode:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	xorq %rdi, %rdi
	mov %esp, %esp

	# Call constructors from global objects
	call initialiseConstructors

	# Transfer control to the main kernel.
	jmp kernel_main
.code32
# Print error with the error code
error:
	movl $0x4f524f45, 0xb8000
	movl $0x4f3a4f52, 0xb8004
	movl $0x4f204f20, 0xb8008
	movb %al		, 0xb800a
	hlt
# Check if our bootloader is supporting multiboot
check_multiboot:
	cmp $0x36D76289, %eax	# this is a MB2 compatible bootloader
	jne .no_multiboot
	ret
.no_multiboot:
	movb $'0', %al
	jmp error
# Check if cpuid is available
check_cpuid:
	# Copy FLAGS in to EAX via stack
	pushfd
	pop %eax
	# Copy to ECX as well for comparing later on
	mov %eax, %ecx
	# Flip the ID bit
	xor $(1 << 21), %eax   # MDP: Missing $
	# Copy EAX to FLAGS via the stack
	push %eax
	popfd
	# Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
	pushfd
	pop %eax
	# Restore FLAGS from the old version stored in ECX (i.e. flipping the ID bit
	# back if it was ever flipped).
	push %ecx
	popfd
	# Compare EAX and ECX. If they are equal then that means the bit wasn't
	# flipped, and CPUID isn't supported.
	cmp %ecx, %eax
	je .no_cpuid
	ret
.no_cpuid:
	mov $'1', %al
	jmp error
check_long_mode:
	# test if extended processor info in available
	mov $0x80000000,%eax	# implicit argument for cpuid
	cpuid                  	# get highest supported argument
	cmp $0x80000001,%eax   	# it needs to be at least 0x80000001
	jb .no_long_mode       	# if it's less, the CPU is too old for long mode

	# use extended info to test if long mode is available
	mov $0x80000001,%eax   	# argument for extended processor info
	cpuid                  	# returns various feature bits in ecx and edx
	test $(1 << 29),%edx      	# test if the LM-bit is set in the D-register
	                        # MDP: Missing $

	jz .no_long_mode       	# If it's not set, there is no long mode
	ret
.no_long_mode:
	mov $'2',%al
	jmp error
set_up_page_tables:
	# map first P4 entry to P3 table
	mov $p3_table, %eax
	or $0b11, %eax # present + writable
	mov %eax, (p4_table)

	# map first P3 entry to P2 table
	mov $p2_table, %eax
	or $0b11, %eax # present + writable
	mov %eax, (p3_table)

	# map each P2 entry to a huge 2MiB page
	mov $0, %ecx # counter variable
.map_p2_table:
	# map ecx-th P2 entry to a huge page that starts at address (2MiB * ecx)
	mov $0x200000, %eax                 # 2MiB
	mul %ecx                            # start address of ecx-th page
	or  $0b10000011, %eax               # present + writable + huge
	mov %eax, p2_table(,%ecx,8)    # map ecx-th entry

	inc %ecx            # increase counter
	cmpl $512, %ecx      # if counter == 512, the whole P2 table is mapped
	jne .map_p2_table   # else map the next entry

	ret

enable_paging:
	# load P4 to cr3 register (cpu uses this to access the P4 table)
	mov $p4_table, %eax
	mov %eax, %cr3
	# enable PAE-flag in cr4 (Physical Address Extension)
	mov %cr4, %eax
	or $(1 << 5),%eax     # MDP: Missing $
	mov %eax, %cr4

	# set the long mode bit in the EFER MSR (model specific register)
	mov $0xC0000080, %ecx
	rdmsr
	or $(1 << 8), %eax        # MDP: Missing $
	wrmsr

	# enable paging in the cr0 register
	mov %cr0, %eax
	or $(1 << 31), %eax            # MDP: Missing $
	mov %eax, %cr0

	ret
.size _start, . - _start

.section .data
	// Die GDT besteht nur aus 4 Deskriptoren: Null, 32-Bit-Code, Daten und
	// 64-Bit-Code
	init_gdt:
		// Null-Deskriptor
		.quad 0

		// 32-Bit-Code-Deskriptor
		.word 0xFFFF
		.word 0x0000
		.byte 0x00
		.byte 0x98
		.byte 0xCF
		.byte 0x00

		// Daten-Deskriptor
		.word 0xFFFF
		.word 0x0000
		.byte 0x00
		.byte 0x92
		.byte 0xCF
		.byte 0x00

		// 64-Bit-Code-Deskriptor
		.int 0
		.byte 0
		.byte 0x98
		.byte 0x20
		.byte 0

	init_gdtr:
		.word 4 * 8 - 1    # Size should have 1 subtracted from it
		.quad init_gdt