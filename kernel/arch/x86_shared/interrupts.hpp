#pragma once
#include <stdint.h>

static bool interrupts_disabled = true;

inline void disable_interrupts() {
  // disable interrupts
  if (interrupts_disabled == false) {
    asm volatile("cli");
    interrupts_disabled = true;
  }
}

inline void enable_interrupts() {
  // enable interrupts
  if (interrupts_disabled == true) {
    asm volatile("sti");
    interrupts_disabled = false;
  }
}

template <class T, typename... F>
inline void without_interrupts(T lambda, F &&... args) {
  disable_interrupts();
  lambda(args...);
  enable_interrupts();
}

struct InterruptFrame {
  uintptr_t ip;
  uintptr_t cs;
  uintptr_t flags;
  uintptr_t sp;
  uintptr_t ss;
};

struct InterruptFlags {
  uint8_t type : 4;
  uint8_t zero : 1;
  uint8_t dpl : 2;
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