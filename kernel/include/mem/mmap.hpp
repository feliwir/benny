#pragma once
#include "multiboot.h"
#include <stdint.h>
#include <vga.hpp>

struct MemoryInfo {
  uint64_t addr;
  uint64_t length;
  uint32_t type;
};

class MMap {
public:
  void Initialize(multiboot_tag_mmap *tag);
  void Print(Vga term);

  uintptr_t UsableMemory();

  [[nodiscard]] inline uint16_t GetNumInfos() const { return m_infoSize; }
  inline MemoryInfo &GetInfo(uint16_t i) { return m_info[i]; }

private:
  void Sort();

  static constexpr uint8_t MEMORY_INFO_MAX = 16;
  MemoryInfo m_info[MEMORY_INFO_MAX];
  uint16_t m_infoSize;
};