#include "mmap.hpp"

#define MEMORY_INFO_MAX 16

MemoryInfo memory_info[MEMORY_INFO_MAX];
uint16_t memory_info_size;


void SortMMap() {
	for (int i = 1; i < memory_info_size; ++i) {
		for (int k = i; k != 0 && memory_info[k].addr < memory_info[k - 1].addr; --k) {
			MemoryInfo temp(memory_info[k]);
			memory_info[k] = memory_info[i];
			memory_info[i] = temp;
		}
	}
}


void InitMMap(multiboot_tag_mmap *tag) {
	memory_info_size = 0;

	uint32_t entry_size = tag->entry_size;
	multiboot_memory_map_t *entry = tag->entries;

	while ((reinterpret_cast<uintptr_t>(entry) - reinterpret_cast<uintptr_t>(tag)) + entry_size <= tag->size) {
		for (int i = 0; i < MEMORY_INFO_MAX; ++i) {
			// Check if the entries are contiguous
			if (memory_info[i].addr < entry->addr
				&& (memory_info[i].addr + memory_info[i].length >= entry->addr)
				&& memory_info[i].type == entry->type) {
				// Current entry comes after this one
				uint64_t difference = memory_info[i].addr + memory_info[i].length - entry->addr;
				memory_info[i].length += (entry->len - difference);
				goto loop_end;
			}
			if (entry->addr < memory_info[i].addr
				&& (entry->addr + entry->len >= memory_info[i].addr)
				&& memory_info[i].type == entry->type) {
				// Current entry comes before this one
				memory_info[i].addr = entry->addr;
				goto loop_end;
			}
		}


		if (memory_info_size == MEMORY_INFO_MAX) {
			// TODO: log this
			
		} else {
			memory_info[memory_info_size].addr = entry->addr;
			memory_info[memory_info_size].length = entry->len;
			memory_info[memory_info_size].type = entry->type;

			memory_info_size++;
		}

loop_end:
		entry = reinterpret_cast<multiboot_memory_map_t*>(reinterpret_cast<uintptr_t>(entry) + entry_size);
	}

	SortMMap();
}