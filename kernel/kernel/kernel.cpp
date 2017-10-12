#include <arch.hpp>
#include <assert.hpp>
#include <vga.hpp>

Vga terminal;
Arch architecture;

extern "C" void kernel_main(void) 
{
  architecture.Initialize();
  terminal.Clear();
  terminal << "Hello kernel!\n" << architecture.GetArchitecture() << "\n";
  while (true) {
  }
}
