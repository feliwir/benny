#include "paging.hpp"
#include "pmm.hpp"
#include <util/util.hpp>

// Paging register manipulation macro
#define SET_PGBIT(cr0) (cr0 = cr0 | 0x80000000)
#define CLEAR_PSEBIT(cr4) (cr4 = cr4 & 0xffffffef)

PageDirectory *Paging::s_pageDir = nullptr;
bool Paging::s_enabled = false;
uintptr_t Paging::s_memoryOffset = 0;

void Paging::Initialize() {
  // Place paging data after pmm bitmap
  s_memoryOffset = PMM::GetBitmap().GetEnd();
  s_pageDir = SimpleAlloc<PageDirectory>();
  memset(s_pageDir, 0, sizeof(PageDirectory));

  // Now, map 4mb begining from 0xC0000000 to 0xC0400000(should corresponding to
  // first 1024 physical blocks, so MAKE SURE pmm bitmap is all clear at this
  // point)
  uint32_t i = LOAD_MEMORY_ADDRESS;
  while (i < LOAD_MEMORY_ADDRESS + 4 * M) {
    AllocatePage(i, 0, 1, 1);
    i = i + PAGE_SIZE;
  }
  // Map some memory after 0xc0400000 as kernel heeap ? do it later.
  i = LOAD_MEMORY_ADDRESS + 4 * M;
  while (i < LOAD_MEMORY_ADDRESS + 4 * M + KHEAP_INITIAL_SIZE) {
    AllocatePage(i, 0, 1, 1);
    i = i + PAGE_SIZE;
  }

  Enable();
}

void Paging::AllocatePage(uintptr_t virtual_addr, uint32_t frame, int is_kernel,
                          int is_writable) {
  if (s_pageDir == nullptr) {
    // TODO: log an error
    return;
  }

  uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr),
           page_tbl_idx = PAGETBL_INDEX(virtual_addr);

  PageTable *&table = s_pageDir->ref_tables[page_dir_idx];

  // Table does not exist. Allocate it
  if (table == nullptr) {
    table = SimpleAlloc<PageTable>();

    uintptr_t t =
        (uintptr_t)GetPhysicalAdress(reinterpret_cast<uintptr_t>(table));
    s_pageDir->tables[page_dir_idx].frame = t >> 12;
    s_pageDir->tables[page_dir_idx].present = 1;
    s_pageDir->tables[page_dir_idx].rw = 1;
    s_pageDir->tables[page_dir_idx].user = 1;
    s_pageDir->tables[page_dir_idx].page_size = 0;
  }

  // If the coressponding page does not exist, allocate_block!
  if (!table->pages[page_tbl_idx].present) {
    uint32_t t;
    // Normally, we'll allocate frames from physical memory manager, but
    // sometimes it's useful to be able to set any frame(for example, share
    // memory between process)
    t = PMM::AllocBlock();
    table->pages[page_tbl_idx].frame = t;
    table->pages[page_tbl_idx].present = 1;
    table->pages[page_tbl_idx].rw = 1;
    table->pages[page_tbl_idx].user = 1;
  }
}

uintptr_t Paging::GetPhysicalAdress(uintptr_t virtual_addr) {
  if (!s_enabled) {
    return virtual_addr - LOAD_MEMORY_ADDRESS;
  }
  uint32_t page_dir_idx = PAGEDIR_INDEX(virtual_addr),
           page_tbl_idx = PAGETBL_INDEX(virtual_addr),
           page_frame_offset = PAGEFRAME_INDEX(virtual_addr);

  PageTable *table = s_pageDir->ref_tables[page_dir_idx];

  if (table == nullptr) {
    // TODO: log an error
    return 0;
  }
  if (!table->pages[page_tbl_idx].present) {
    // TODO: log an error
    return 0;
  }
  uint32_t t = table->pages[page_tbl_idx].frame;
  return (t << 12) + page_frame_offset;
}

void Paging::Enable() {
  uint32_t cr0, cr4;

  asm volatile("mov %0, %%cr3" :: "r"(s_pageDir));

  asm volatile("mov %%cr4, %0" : "=r"(cr4));
  CLEAR_PSEBIT(cr4);
  asm volatile("mov %0, %%cr4" ::"r"(cr4));

  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  SET_PGBIT(cr0);
  asm volatile("mov %0, %%cr0" ::"r"(cr0));

  s_enabled = true;
}