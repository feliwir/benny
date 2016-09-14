.code32
.global error
.section .text
# Print error with the error code
error:
	movl $0x4f524f45, 0xb8000
	movl $0x4f3a4f52, 0xb8004
	movl $0x4f204f20, 0xb8008
	movb %al		, 0xb800a
	hlt