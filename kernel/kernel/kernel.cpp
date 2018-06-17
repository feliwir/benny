#include <arch.hpp>
#include <assert.hpp>
#include <mmap.hpp>
#include <multiboot.h>
#include <stdint.h>
#include <vga.hpp>

Vga terminal;
Arch architecture;
MMap memoryMap;

void processMultiboot(multiboot_tag *tag) {
  uint32_t total_size = tag->type;
  terminal << "Total multiboot tags: " << total_size << "\n";
  tag += MULTIBOOT_TAG_ALIGN;

  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    terminal << "Processing tag: " << tag->type << "\n";

    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MMAP: // Memory map
    {
      multiboot_tag_mmap *tag_mmap =
          reinterpret_cast<multiboot_tag_mmap *>(tag);
      terminal << "Found memory map tag!"
               << "\n";
      memoryMap.Initialize(tag_mmap);
    } break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
      terminal << "Found basic memory info!"
               << "\n";
      break;
    }
    tag += ((tag->size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1));
  }

  terminal << "Done processing tags!"
           << "\n";
}

extern "C" void kernel_main(multiboot_tag *tag) {
  architecture.Initialize();
  terminal.Clear();

  terminal << "Architecture: " << architecture.GetArchitecture() << "\n";

  processMultiboot(tag);

  asm volatile("hlt");
}
