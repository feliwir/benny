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

.global check_multiboot
.section .text
# Check if our bootloader is supporting multiboot
check_multiboot:
    cmp $0x36D76289, %eax	# this is a MB2 compatible bootloader
    jne .no_multiboot
    ret
.no_multiboot:
    movb $'0', %al
    jmp error