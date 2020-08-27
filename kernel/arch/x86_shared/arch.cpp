#include "descriptor_tables/gdt.hpp"
#include "descriptor_tables/idt.hpp"
#include "drivers/pic.hpp"
#include "mem/pmm.hpp"
#include <arch.hpp>
#include <mem/mmap.hpp>

void Arch::Initialize(MMap &mmap) {
  GDT::Initialize();
  IDT::Initialize();
  PIC::Initialize();
  PMM::Initialize(mmap);
}