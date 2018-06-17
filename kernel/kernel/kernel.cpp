#include <arch.hpp>
#include <assert.hpp>
#include <mmap.hpp>
#include <multiboot.h>
#include <stdint.h>
#include <util.hpp>
#include <vga.hpp>

Vga terminal;
Arch architecture;
MMap memoryMap;

void processMultiboot(multiboot_tag *tag) {
  uint32_t total_size = tag->type;
  terminal << "Total tags size: " << total_size << "\n";
  advancePtr(tag, MULTIBOOT_TAG_ALIGN);

  terminal << (int)sizeof(multiboot_tag) << "\n";

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
    auto next =
        ((tag->size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1));
    advancePtr(tag, next);
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
