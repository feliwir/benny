#include <vga.hpp>
#include <arch.hpp>

Vga terminal;
Arch architecture;

extern "C" /* Use C linkage for kernel_main. */
    void
    kernel_main(void) {
  terminal.Clear();
  terminal.Write("Hello kernel!");
  terminal.Write(architecture.GetArchitecture());
  while (true) {
  }
}
