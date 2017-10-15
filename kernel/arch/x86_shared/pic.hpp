#pragma once
#include <stdint.h>

struct InterruptDescriptor
{
    uint16_t offset;
    uint16_t selector;
    uint8_t ist_index : 2;
    uint8_t reserved :6;
    uint8_t type : 3;
    uint8_t sizeType : 1; //32-bit or 16-bit, in LM: 64-bit
    uint8_t dpl : 2;        //descriptor privelege level
    uint8_t used :1;    //is this used?
    uint16_t offset2;
    #ifdef _x86_64_
    uint32_t offset3;
    uint32_t reserved2;
    #endif
} __attribute__((packed));

#ifdef _x86_64_
static_assert(sizeof(InterruptDescriptor) == 16,
"InterruptDescriptor must be 16 bytes large in Longmode.");
#elif _i386_
static_assert(sizeof(InterruptDescriptor) == 8,
"InterruptDescriptor must be 8 bytes large in 32-bit mode.");
#endif


class PIC {
public:
  static void Initialize();

private:
    static const uint32_t s_length = 256;
    static InterruptDescriptor s_descriptors[s_length];
  PIC() = delete;
};