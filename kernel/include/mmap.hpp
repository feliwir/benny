#pragma once
#include "multiboot.h"
#include <stdint.h>

class MMap {
public:
  void Initialize(multiboot_tag_mmap *tag);

  struct MemoryInfo {
    uint64_t addr;
    uint64_t length;
    uint32_t type;
  };

private:
  void Sort();

  static constexpr uint8_t MEMORY_INFO_MAX = 16;
  MemoryInfo m_info[MEMORY_INFO_MAX];
  uint16_t m_infoSize;
};