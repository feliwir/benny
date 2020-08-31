#include "pmm.hpp"
#include "paging.hpp"
#include "vga.hpp"
#include <mem/mmap.hpp>
#include <scheduler/lock_guard.hpp>
#include <util/math.hpp>
#include <util/util.hpp>

Bitmap PMM::bitmap;
uintptr_t PMM::highestPage = 0;
TicketLock PMM::s_lock;
uintptr_t PMM::s_lastUsedIndex = 0;

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

  size_t bitmap_size = kmath::div_roundup(highestPage, PAGE_SIZE) / 8;

  // Second, find a location with enough free pages to host the bitmap.
  for (size_t i = 0; i < mmap.GetNumInfos(); i++) {
    auto info = mmap.GetInfo(i);
    if (info.type != MULTIBOOT_MEMORY_AVAILABLE)
      continue;

    if (info.length >= bitmap_size) {
      auto *bitmap_addr = (uint8_t *)(info.addr + 0);

      // Initialise entire bitmap to 1 (non-free)
      memset(bitmap_addr, 0xff, bitmap_size);

      bitmap.SetData(bitmap_addr, bitmap_size);

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

void *PMM::InnerAlloc(uintptr_t count, uintptr_t limit) {
  uintptr_t p = 0;

  while (s_lastUsedIndex < limit) {
    if (!bitmap.IsSet(s_lastUsedIndex++)) {
      if (++p == count) {
        uintptr_t page = s_lastUsedIndex - count;
        for (uintptr_t i = page; i < s_lastUsedIndex; i++) {
          bitmap.Set(i);
        }
        return (void *)(page * PAGE_SIZE);
      }
    } else {
      p = 0;
    }
  }

  return nullptr;
}

uintptr_t PMM::AllocBlock() {
  TicketLockGuard(s_lock);
  uintptr_t limit = highestPage / PAGE_SIZE;

  while (s_lastUsedIndex < limit) {
    if (!bitmap.IsSet(s_lastUsedIndex++)) {
      return s_lastUsedIndex;
    }
  }

  return 0;
}

void *PMM::Alloc(uintptr_t count) {
  TicketLockGuard(s_lock);
  uintptr_t l = s_lastUsedIndex;
  void *ret = InnerAlloc(count, highestPage / PAGE_SIZE);

  if (ret == nullptr) {
    // reset our last used index
    s_lastUsedIndex = 0;
    ret = InnerAlloc(count, l);
  }

  return ret;
}

void *PMM::AllocZ(uintptr_t count) {
  void *ret = Alloc(count);

  if (ret == nullptr) {
    return nullptr;
  }

  memset(ret, 0, PAGE_SIZE);

  return ret;
}