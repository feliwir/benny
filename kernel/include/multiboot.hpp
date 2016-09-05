#pragma once
#include <stdint.h>

constexpr uint32_t MULTIBOOT_MAGIC = 0x2BADB002;

struct Multiboot {
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t bootdevce;
  uint32_t cmdline;
  uint32_t module_count;
  uint32_t module_address;
  /* etc... */
} PACKED;
