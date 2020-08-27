#include <arch.hpp>
#include "descriptor_tables/gdt.hpp"
#include "descriptor_tables/idt.hpp"
#include "drivers/pic.hpp"
#include "mem/pmm.hpp"
#include <vga.hpp>

void Arch::Initialize(multiboot_tag* bootInfo) {
  GDT::Initialize();
  IDT::Initialize();
  PIC::Initialize();
  PMM::Initialize(bootInfo);
}