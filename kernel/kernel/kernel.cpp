#include <vga.hpp>

Vga terminal;

extern "C" /* Use C linkage for kernel_main. */
    void
    kernel_main(void) {
  terminal.Clear();
  terminal.Write("Hello kernel!");
}
