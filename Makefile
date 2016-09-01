SHELL = /bin/bash
SUBDIRS = kernel
OS      := benny
ARCH    ?= i386

.PHONY: all
all: $(OS).iso

$(OS).iso: $(SUBDIRS) tmpclean
	mkdir -p iso/boot/grub
	cp kernel/kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(OS).iso iso

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: tmpclean
tmpclean:
	rm -f $(OS).iso

.PHONY: doc
doc:
	$(MAKE) -C kernelmode $@

.PHONY: clean
clean: tmpclean
	rm -rf iso
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done

