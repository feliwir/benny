#include "gdt.hpp"
#include "idt.hpp"
#include "pic.hpp"
#include <arch.hpp>
#include <vga.hpp>

void Arch::Initialize() {
  GDT::Initialize();
  IDT::Initialize();
  PIC::Initialize();
}