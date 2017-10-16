
#include "idt.hpp"

InterruptDescriptor IDT::s_descriptors[IDT::s_length];

void IDT::Initialize() {
  struct {
    uint16_t limit;
    void *pointer;
  } __attribute__((packed)) idtp = {
      .limit = IDT::s_length * sizeof(InterruptDescriptor) - 1,
      .pointer = &IDT::s_descriptors,
  };

  asm volatile("lidt %0" : : "m"(idtp));
}

void IDT::AddHandler(int index, void (*func)(), int sel, GateType g) {
  uintptr_t addr = reinterpret_cast<uintptr_t>(func);
  auto& d = IDT::s_descriptors[index];
  d.offset_1 = (addr & 0xFFFF);
  d.offset_2 = (addr >> 16) & 0xFFFF;
  d.selector = sel;
  d.type = g;
  d.used = 1;
  d.dpl = 0;

  // other fields
  d.reserved = 0;
  d.ist_index = 0;
}