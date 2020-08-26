#pragma once
#include "../util/interrupts.hpp"

#ifdef _x86_64_
static_assert(sizeof(InterruptDescriptor) == 16,
              "InterruptDescriptor must be 16 bytes large in Longmode.");
#elif _i386_
static_assert(sizeof(InterruptDescriptor) == 8,
              "InterruptDescriptor must be 8 bytes large in 32-bit mode.");
#endif

class IDT {
public:
  IDT() = delete;
  static void Initialize();

  static void EmptyHandler(int index);
  static void AddHandler(int index, void (*func)(InterruptFrame *), int sel,
                         InterruptFlags flags);
  static void AddHandler(int index, void (*func)(InterruptFrame *, uintptr_t),
                         int sel, InterruptFlags flags);
  static void AddInterruptHandler(int index, void (*func)(InterruptFrame *));

private:
  static const uint32_t s_length = 256;
  static InterruptDescriptor s_descriptors[s_length];
};