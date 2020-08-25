#include <arch.hpp>
#include <assert.hpp>
#include <multiboot.h>
#include <stdint.h>
#include <vga.hpp>

import mmap;
import util;

Vga term;
Arch architecture;
MMap memoryMap;

void processMultiboot(multiboot_tag *tag) {
  uint32_t total_size = tag->type;
  term << "Total tags size: " << total_size << term.endl;
  advancePtr(tag, MULTIBOOT_TAG_ALIGN);

  term << (int)sizeof(multiboot_tag) << term.endl;

  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MMAP: // Memory map
    {
      auto *tag_mmap = reinterpret_cast<multiboot_tag_mmap *>(tag);
      term << "Found memory map tag!" << term.endl;
      memoryMap.Initialize(tag_mmap);
    } break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
      term << "Found basic memory info!" << term.endl;
      break;
    }
    auto next =
        ((tag->size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1));
    advancePtr(tag, next);
  }

  term << "Done processing tags!" << term.endl;
}

extern "C" void kernel_main(multiboot_tag *tag) {
  architecture.Initialize();
  term.Clear();
  term << "Hello world!" << term.endl;
  term << "Architecture: " << architecture.GetArchitecture() << term.endl;
  processMultiboot(tag);

  while (true) {
    for (int i = 0; i < 1000000; i++)
      asm volatile("pause");
    term << '#';
  }

  asm volatile("hlt");
}
