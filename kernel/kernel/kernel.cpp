#include <arch.hpp>
#include <assert.hpp>
#include <vga.hpp>

Vga terminal;
Arch architecture;

extern "C" /* Use C linkage for kernel_main. */
    void
    kernel_main(void) {
  architecture.Initialize();
  terminal.Clear();
  terminal << "Hello kernel!\n" << architecture.GetArchitecture() << "\n";
  while (true) {
  }
}
