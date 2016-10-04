include make.config
include $(ARCHDIR)/make.config

OBJS:=\
$(ARCH_OBJS) \
kernel/kernel.o \
kernel/string.o \
kernel/util.o \
kernel/vga.o \
kernel/cxx.o \

.PHONY: all
all: kernel.bin

kernel.bin: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CPPFLAGS)

%.o: %.asm
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f kernel.bin $(OBJS) *.o */*.o */*/*.o
