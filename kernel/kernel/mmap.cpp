#include "mmap.hpp"

void MMap::Sort() {
  for (int i = 1; i < m_infoSize; ++i) {
    for (int k = i; k != 0 && m_info[k].addr < m_info[k - 1].addr; --k) {
      MemoryInfo temp(m_info[k]);
      m_info[k] = m_info[i];
      m_info[i] = temp;
    }
  }
}

void MMap::Initialize(multiboot_tag_mmap *tag) {
  m_infoSize = 0;

  uint32_t entry_size = tag->entry_size;
  multiboot_memory_map_t *entry = tag->entries;

  while (
      (reinterpret_cast<uintptr_t>(entry) - reinterpret_cast<uintptr_t>(tag)) +
          entry_size <=
      tag->size) {
    for (auto &info : m_info) {
      // Check if the entries are contiguous
      if (info.addr < entry->addr && (info.addr + info.length >= entry->addr) &&
          info.type == entry->type) {
        // Current entry comes after this one
        uint64_t difference = info.addr + info.length - entry->addr;
        info.length += (entry->len - difference);
        goto loop_end;
      }
      if (entry->addr < info.addr && (entry->addr + entry->len >= info.addr) &&
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