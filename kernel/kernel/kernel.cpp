#include <arch.hpp>
#include <assert.hpp>
#include <emmintrin.h>
#include <multiboot.h>
#include <stdint.h>
#include <vga.hpp>

import mmap;
import util;

Arch architecture;


extern "C" void kernel_main(multiboot_tag *tag) {
  architecture.Initialize(tag);

  while (true) {
    asm volatile("hlt");
  }
}
