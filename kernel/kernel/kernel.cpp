#include <multiboot.hpp>
#include <vga.hpp>

Vga terminal;

extern "C" /* Use C linkage for kernel_main. */
    void
    kernel_main(const Multiboot &multiboot, uint32_t magic) {
  if (magic != MULTIBOOT_MAGIC) {
    terminal.Write("Invalid multiboot magic!");
    return;
  }
  terminal.Clear();
  terminal.Write("Hello kernel!");
}
