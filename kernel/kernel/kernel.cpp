#include <arch.hpp>
#include <emmintrin.h>
#include <mem/mmap.hpp>
#include <multiboot.h>
#include <stdint.h>
#include <util/assert.hpp>
#include <util/util.hpp>
#include <vga.hpp>

Arch architecture;
MMap memoryMap;

void processMultiboot(multiboot_tag *tag) {
  advancePtr(tag, MULTIBOOT_TAG_ALIGN);

  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MMAP: // Memory map
    {
      auto *tag_mmap = reinterpret_cast<multiboot_tag_mmap *>(tag);
      memoryMap.Initialize(tag_mmap);
    } break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
      // TODO:
      // auto *tag_meminfo = reinterpret_cast<multiboot_tag_basic_meminfo
      // *>(tag);
      break;
    }
    auto next =
        ((tag->size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1));
    advancePtr(tag, next);
  }
}

extern "C" void kernel_main(multiboot_tag *tag) {
  processMultiboot(tag);
  architecture.Initialize(memoryMap);

  while (true) {
    asm volatile("hlt");
  }
}
