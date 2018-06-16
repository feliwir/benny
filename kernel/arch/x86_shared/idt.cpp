
#include "idt.hpp"
#include "gdt.hpp"
#include <exception.hpp>

InterruptDescriptor IDT::s_descriptors[IDT::s_length];

__attribute__((interrupt)) void Exception_DivByZero(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Divison by zero!" << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_Debug(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Debug interrupt!\n";
  DumpRegisters(screen,frame);
}

__attribute__((interrupt)) void Exception_Breakpoint(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Breakpoint triggered!\n";
  DumpRegisters(screen,frame);
}

__attribute__((interrupt)) void Exception_Overflow(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Trap: Overflow!\n";
  DumpRegisters(screen,frame);
}

__attribute__((interrupt)) void Exception_OutOfBounds(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Bound range exceeded!" << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_InvalidOp(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Invalid opcode!" << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_DeviceNA(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Device not available!" << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_DoubleFault(InterruptFrame *frame,uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Double Fault! Code: "<< static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_InvalidTSS(InterruptFrame *frame,uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Invalid TSS! Code: "<< static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_SegNotPres(InterruptFrame *frame,uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Present bit for segment not set! Code: "<< static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_StackSeg(InterruptFrame *frame,uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Stack segment fault! Code: "<< static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_GenProtFault(InterruptFrame *frame,uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: General protection fault! Code: "<< static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

__attribute__((interrupt)) void Exception_PageFault(InterruptFrame *frame,uintptr_t err) {
  Vga screen;
  screen.Clear();
  screen << "Fault: Page fault! Code: "<< static_cast<unsigned int>(err) << "\n";
  DumpRegisters(screen,frame);
  Panic(screen,"Cannot continue after fatal error!");
}

void IDT::Initialize() {
  IDT::AddHandler(0, &Exception_DivByZero,    CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(1, &Exception_Debug,        CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(3, &Exception_Breakpoint,   CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(4, &Exception_Overflow,     CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(5, &Exception_OutOfBounds,  CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(6, &Exception_InvalidOp,    CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(7, &Exception_DeviceNA,     CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(8, &Exception_DoubleFault,  CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(10,&Exception_InvalidTSS,   CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(11,&Exception_SegNotPres,   CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(12,&Exception_StackSeg,     CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(13,&Exception_GenProtFault, CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  IDT::AddHandler(14,&Exception_PageFault,    CODE_SELECTOR, {SEG_INTERRUPT_GATE, 0, 0, 1});
  
  struct {
    uint16_t limit;
    void *pointer;
  } __attribute__((packed)) idtp = {
      .limit = IDT::s_length * sizeof(InterruptDescriptor) - 1,
      .pointer = &IDT::s_descriptors,
  };

  asm volatile("lidt %0" : : "m"(idtp));
}

void IDT::AddHandler(int index, void (*func)(InterruptFrame *), int sel, InterruptFlags f) {
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

void IDT::AddHandler(int index, void (*func)(InterruptFrame *,uintptr_t), int sel, InterruptFlags f) {
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