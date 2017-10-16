#pragma once
#include <stdint.h>

struct InterruptFrame {
  uintptr_t ip;
  uintptr_t cs;
  uintptr_t flags;
  uintptr_t sp;
  uintptr_t ss;
};

enum GateType : uint8_t {
  GT_32bitTask = 0x5,
  GT_16bitInt = 0x6,
  GT_16bitTrap = 0x7,
  GT_32bitInt = 0xE,
  GT_32bitTrap = 0xF
};

struct InterruptDescriptor {
  uint16_t offset_1;
  uint16_t selector;
  uint8_t ist_index : 2; // LM only
  uint8_t reserved : 6;
  uint8_t type : 4; // type
  uint8_t dpl : 2;  // descriptor privelege level
  uint8_t used : 1; // is this used?
  uint16_t offset_2;
#ifdef _x86_64_
  uint32_t offset_3;
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

class IDT {
public:
  static void Initialize();

  static void AddHandler(int index, void (*func)(InterruptFrame *), int sel,
                         GateType g);

private:
  static const uint32_t s_length = 256;
  static InterruptDescriptor s_descriptors[s_length];
  IDT() = delete;
};