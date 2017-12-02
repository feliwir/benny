#pragma once
#include "multiboot.h"
#include <stdint.h>

void InitMMap(multiboot_tag_mmap *tag);



struct MemoryInfo {
	uint64_t addr;
	uint64_t length;
	uint32_t type;
};