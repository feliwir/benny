#include <arch.hpp>
#include <assert.hpp>
#include <multiboot.h>
#include <stdint.h>
#include <vga.hpp>
#include <mmap.hpp>

Vga terminal;
Arch architecture;
MMap memoryMap;

void process_multiboot(multiboot_tag *tag) {
	uint32_t total_size = tag->type;
	tag += 8;

	while (tag->type != 0) {
    terminal << "Processing tag: " << tag->type << "\n";

		switch (tag->type) {
		case 6: // Memory map
			multiboot_tag_mmap *tag_mmap = reinterpret_cast<multiboot_tag_mmap*>(tag);
      terminal << "Found memory map tag!" << "\n";
			memoryMap.Initialize(tag_mmap);
		}
		tag = tag + ((tag->size + 7) / 8);
	}
}


extern "C" void kernel_main(multiboot_tag *tag) {
  architecture.Initialize();
  terminal.Clear();

  terminal << "Architecture: " << architecture.GetArchitecture() << "\n";

  process_multiboot(tag);
  
  asm volatile("hlt");
}
