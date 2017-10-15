#include <arch.hpp>
#include <assert.hpp>
#include <vga.hpp>
#include <stdint.h>
#include <multiboot.h>

Vga terminal;
Arch architecture;

extern "C" void kernel_main(uint32_t addr) {
  architecture.Initialize();
  terminal.Clear();
  terminal << "Hello kernel!\n" << architecture.GetArchitecture() << "\n";

  while (true) {
  }
}
