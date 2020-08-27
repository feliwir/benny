#include "pmm.hpp"
#include "paging.hpp"
#include "vga.hpp"
#include <mem/mmap.hpp>
#include <util/math.hpp>
import util;

Bitmap PMM::bitmap;
uintptr_t PMM::highestPage = 0;
TicketLock PMM::s_lock;

// Defined in linker.ld, indicate the end of kernel code/data
extern uint32_t end;

void PMM::Initialize(MMap &mmap) {
  Vga term;
  term.Clear();

  mmap.Print(term);

  uintptr_t usableMemory = mmap.UsableMemory();

  term << "Total memory: " << usableMemory;

  // First, calculate how big the bitmap needs to be.
  for (size_t i = 0; i < mmap.GetNumInfos(); i++) {
    auto info = mmap.GetInfo(i);
    if (info.type != MULTIBOOT_MEMORY_AVAILABLE)
      continue;

    uintptr_t top = info.addr + info.length;

    if (top > highestPage)
      highestPage = top;
  }

  size_t bitmap_size = div_roundup(highestPage, PAGE_SIZE) / 8;

  // Second, find a location with enough free pages to host the bitmap.
  for (size_t i = 0; i < mmap.GetNumInfos(); i++) {
    auto info = mmap.GetInfo(i);
    if (info.type != MULTIBOOT_MEMORY_AVAILABLE)
      continue;

    if (info.length >= bitmap_size) {
      auto *bitmap_addr = (uint8_t *)(info.addr + 0);

      // Initialise entire bitmap to 1 (non-free)
      memset(bitmap_addr, 0xff, bitmap_size);

      bitmap.SetData(bitmap_addr);

      info.length -= bitmap_size;
      info.addr += bitmap_size;

      break;
    }

    // Third, populate free bitmap entries according to memory map.
    for (size_t i = 0; i < mmap.GetNumInfos(); i++) {
      auto info = mmap.GetInfo(i);

      if (info.type != MULTIBOOT_MEMORY_AVAILABLE)
        continue;

      for (uintptr_t j = 0; j < info.length; j += PAGE_SIZE)
        bitmap.Unset((info.addr + j) / PAGE_SIZE);
    }
  }
}

uint32_t PMM::AllocateBlock() {
  uint32_t free_block = FirstFreeBlock();
  return free_block;
}

void PMM::FreeBlock(uint32_t blockNumber) {}

uint32_t PMM::FirstFreeBlock() {
  uint32_t i;

  return (uint32_t)-1;
}