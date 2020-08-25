
#include "idt.hpp"
#include "cpu_exceptions.hpp"
#include "gdt.hpp"
#include "pic.hpp"

InterruptDescriptor IDT::s_descriptors[IDT::s_length];

Vga screen;

__attribute__((interrupt)) void TimerInterrupt(InterruptFrame *frame) {
  // Do something

  PIC::NotifyEndOfInterrupt((int)HardwareInterrupts::Timer);
}

void IDT::Initialize() {
  IDT::AddHandler(0, &Exception_DivByZero, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(1, &Exception_Debug, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::EmptyHandler(2);
  IDT::AddHandler(3, &Exception_Breakpoint, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(4, &Exception_Overflow, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(5, &Exception_OutOfBounds, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(6, &Exception_InvalidOp, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(7, &Exception_DeviceNA, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(8, &Exception_DoubleFault, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::EmptyHandler(9);
  IDT::AddHandler(10, &Exception_InvalidTSS, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(11, &Exception_SegNotPres, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(12, &Exception_StackSeg, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(13, &Exception_GenProtFault, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(14, &Exception_PageFault, CODE_SELECTOR,
                  {SEG_INTERRUPT_GATE, 0, 0, 1});
  for (uint16_t i = 15; i < 32; i++) {
    IDT::EmptyHandler(i);
  }

  IDT::AddInterruptHandler((int)HardwareInterrupts::Timer, &TimerInterrupt);

  struct {
    uint16_t limit;
    void *pointer;
  } __attribute__((packed)) idtp = {
      .limit = IDT::s_length * sizeof(InterruptDescriptor) - 1,
      .pointer = &IDT::s_descriptors,
  };

  asm volatile("lidt %0" : : "m"(idtp));
}

void IDT::EmptyHandler(int index) {
  auto &d = IDT::s_descriptors[index];
  d.offset_1 = 0;
  d.offset_2 = 0;
#ifdef _x86_64_
  d.offset_3 = 0;
  d.reserved2 = 0;
#endif
  d.selector = 0;
  d.flags = {SEG_INTERRUPT_GATE, 0, 0, 0};
  // other fields
  d.reserved = 0;
  d.ist_index = 0;
}

void IDT::AddHandler(int index, void (*func)(InterruptFrame *), int sel,
                     InterruptFlags f) {
  uintptr_t addr = reinterpret_cast<uintptr_t>(func);
  auto &d = IDT::s_descriptors[index];
  d.offset_1 = (addr & 0xFFFF);
  d.offset_2 = (addr >> 16) & 0xFFFF;
#ifdef _x86_64_
  d.offset_3 = addr >> 32;
  d.reserved2 = 0;
#endif
  d.selector = sel;
  d.flags = f;
  // other fields
  d.reserved = 0;
  d.ist_index = 0;
}

void IDT::AddHandler(int index, void (*func)(InterruptFrame *, uintptr_t),
                     int sel, InterruptFlags f) {
  uintptr_t addr = reinterpret_cast<uintptr_t>(func);
  auto &d = IDT::s_descriptors[index];
  d.offset_1 = (addr & 0xFFFF);
  d.offset_2 = (addr >> 16) & 0xFFFF;
#ifdef _x86_64_
  d.offset_3 = (addr >> 32) & 0xFFFF;
  d.reserved2 = 0;
#endif
  d.selector = sel;
  d.flags = f;
  // other fields
  d.reserved = 0;
  d.ist_index = 0;
}

void IDT::AddInterruptHandler(int index, void (*func)(InterruptFrame *)) {
  AddHandler(index, func, CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
}