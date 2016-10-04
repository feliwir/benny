#include <vga.hpp>
#include <arch.hpp>

Vga terminal;
Arch architecture;

extern "C" /* Use C linkage for kernel_main. */
    void
    kernel_main(void) {
  terminal.Clear();
  terminal<< "Hello kernel!\n" << architecture.GetArchitecture() << "\n";
  while (true) {
  }
}
