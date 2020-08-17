include make.config
include $(ARCHDIR)/make.config

MODULES:=\
kernel/mmap.pcm \
kernel/string.pcm \
kernel/util.pcm

OBJS:=\
$(ARCH_OBJS) \
kernel/kernel.o \
kernel/cxx.o

.PHONY: all
all: kernel.bin

kernel.bin: $(MODULES) $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.pcm: %.cpp
	$(CXX) -c $< -o $@ -Xclang -emit-module-interface $(CPPFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CPPFLAGS) -fprebuilt-module-path=./kernel

%.o: %.asm
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f kernel.bin $(OBJS) *.o */*.o */*/*.o  $(MODULES) *.pcm */*.pcm */*/*.pcm
