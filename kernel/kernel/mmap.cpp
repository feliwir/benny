module;
#include "multiboot.h"
#include <stdint.h>
#include <vga.hpp>

extern Vga term;

export module mmap;

export class MMap {
public:
  void Initialize(multiboot_tag_mmap *tag) {
    m_infoSize = 0;

    uint32_t entry_size = tag->entry_size;
    multiboot_memory_map_t *entry = tag->entries;

    while ((reinterpret_cast<uintptr_t>(entry) -
            reinterpret_cast<uintptr_t>(tag)) +
               entry_size <=
           tag->size) {
      for (auto &info : m_info) {
        // Check if the entries are contiguous
        if (info.addr < entry->addr &&
            (info.addr + info.length >= entry->addr) &&
            info.type == entry->type) {
          // Current entry comes after this one
          uint64_t difference = info.addr + info.length - entry->addr;
          info.length += (entry->len - difference);
          goto loop_end;
        }
        if (entry->addr < info.addr &&
            (entry->addr + entry->len >= info.addr) &&
            info.type == entry->type) {
          // Current entry comes before this one
          info.addr = entry->addr;
          goto loop_end;
        }
      }

      if (m_infoSize == MEMORY_INFO_MAX) {
        // TODO: log this
      } else {
        m_info[m_infoSize].addr = entry->addr;
        m_info[m_infoSize].length = entry->len;
        m_info[m_infoSize].type = entry->type;

        m_infoSize++;
      }

    loop_end:
      entry = reinterpret_cast<multiboot_memory_map_t *>(
          reinterpret_cast<uintptr_t>(entry) + entry_size);
    }

    Sort();
  }

  void Print() {
    term.SetIntegerMode(Vga::IM_HEX);
    for (int i = 0; i < m_infoSize; ++i) {
      auto &info = m_info[i];
      term << "[MMAP:" << i << "] T: " << info.type << " A: " << info.addr
           << " L: " << info.length << term.endl;
    }
  }

  struct MemoryInfo {
    uint64_t addr;
    uint64_t length;
    uint32_t type;
  };

private:
  void Sort() {
    for (int i = 1; i < m_infoSize; ++i) {
      for (int k = i; k != 0 && m_info[k].addr < m_info[k - 1].addr; --k) {
        MemoryInfo temp(m_info[k]);
        m_info[k] = m_info[i];
        m_info[i] = temp;
      }
    }
  }

  static constexpr uint8_t MEMORY_INFO_MAX = 16;
  MemoryInfo m_info[MEMORY_INFO_MAX];
  uint16_t m_infoSize;
};