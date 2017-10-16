#include "gdt.hpp"
#include "idt.hpp"
#include <arch.hpp>
#include <vga.hpp>

__attribute__((interrupt)) void IntHandler_DivByZero(InterruptFrame *frame) {
  Vga screen;
  screen.Clear();
  screen << "Divison by zero! Panic!";
  while (true) {
  }
}

void Arch::Initialize() {
  GDT::Initialize();
  IDT::AddHandler(0, &IntHandler_DivByZero, 0x08, GT_32bitInt);
  IDT::Initialize();
}