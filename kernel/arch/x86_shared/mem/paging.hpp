#pragma once
#include <stdint.h>

constexpr uintptr_t PAGE_SIZE = 4096;
constexpr uintptr_t LOAD_MEMORY_ADDRESS = 0xC0000000;
constexpr uintptr_t KHEAP_INITIAL_SIZE = 48 * 1024 * 1024 + 1024;

// Defone some address calculation macro
#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >> 12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

struct PageDirEntry {
  uint8_t present : 1;
  uint8_t rw : 1;
  uint8_t user : 1;
  uint8_t w_through : 1;
  uint8_t cache : 1;
  uint8_t access : 1;
  uint8_t reserved : 1;
  uint8_t page_size : 1;
  uint8_t global : 1;
  uint8_t available : 3;
  uint32_t frame : 20;
} __attribute__((packed));

struct PageTableEntry {
  uint8_t present : 1;
  uint8_t rw : 1;
  uint8_t user : 1;
  uint8_t reserved : 2;
  uint8_t accessed : 1;
  uint8_t dirty : 1;
  uint8_t reserved2 : 2;
  uint8_t available : 3;
  uint32_t frame : 20;
} __attribute__((packed));

struct PageTable {
  PageTableEntry pages[1024];
} __attribute__((packed));

struct PageDirectory {
  // The actual page directory entries(note that the frame number it stores is
  // physical address)
  PageDirEntry tables[1024];
  // We need a table that contains virtual address, so that we can actually get
  // to the tables
  PageTable *ref_tables[1024];
} __attribute__((packed));

// Defined in paging.c
// PageDirectory kpage_dir;

class Paging {
public:
  static void Initialize();
  static void AllocatePage(uintptr_t virtual_addr, uint32_t frame,
                           int is_kernel, int is_writable);
  static uintptr_t GetPhysicalAdress(uintptr_t virtual_addr);

  template <class T> static inline T *SimpleAlloc() {
    T *result = reinterpret_cast<T *>(s_memoryOffset);
    s_memoryOffset += sizeof(T);
    return result;
  }

private:
  static void Enable();

  static PageDirectory *s_pageDir;
  static uintptr_t s_memoryOffset;
  static bool s_enabled;
};
