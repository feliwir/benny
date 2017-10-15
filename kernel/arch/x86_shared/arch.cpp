#include "gdt.hpp"
#include "pic.hpp"
#include <arch.hpp>

void Arch::Initialize() {
  GDT::Initialize();
  PIC::Initialize();
}