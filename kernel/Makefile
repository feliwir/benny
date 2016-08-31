include make.config
include $(ARCHDIR)/make.config

CRTBEGIN_OBJ:=$(ARCHDIR)/crtbegin.o
CRTEND_OBJ	:=$(ARCHDIR)/crtend.o

OBJS:=\
$(ARCH_OBJS) \
kernel/kernel.o \

ALL_OUR_OBJS:=\
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \

OBJ_LINK_LIST:=\
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \

.PHONY: all
all: kernel.bin

kernel.bin: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld $(CFLAGS) -o $@ $(OBJ_LINK_LIST) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f kernel.bin $(ALL_OUR_OBJS) *.o */*.o */*/*.o
