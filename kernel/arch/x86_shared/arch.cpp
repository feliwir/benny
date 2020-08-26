#include "descriptor_tables/gdt.hpp"
#include "descriptor_tables/idt.hpp"
#include "drivers/pic.hpp"
#include <arch.hpp>
#include <vga.hpp>

void Arch::Initialize() {
  GDT::Initialize();
  IDT::Initialize();
  PIC::Initialize();
}