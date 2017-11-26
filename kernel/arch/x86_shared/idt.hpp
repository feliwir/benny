#pragma once
#include <stdint.h>

struct InterruptFrame {
  uintptr_t ip;
  uintptr_t cs;
  uintptr_t flags;
  uintptr_t sp;
  uintptr_t ss;
};

struct InterruptFlags {
  uint8_t type    : 4;
  uint8_t zero    : 1;
  uint8_t dpl     : 2;
  uint8_t present : 1;
} __attribute__((packed));

struct InterruptDescriptor {
  uint16_t offset_1;
  uint16_t selector;
  uint8_t ist_index : 3; // LM only
  uint8_t reserved : 5;
  InterruptFlags flags;
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

  static void AddHandler(int index, void (*func)(InterruptFrame *), int sel,InterruptFlags flags);
  static void AddHandler(int index, void (*func)(InterruptFrame *,uintptr_t), int sel,InterruptFlags flags);
private:
  static const uint32_t s_length = 256;
  static InterruptDescriptor s_descriptors[s_length];
  IDT() = delete;
};