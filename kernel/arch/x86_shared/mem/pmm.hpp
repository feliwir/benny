#pragma once
#include "bitmap.hpp"
#include <multiboot.h>
#include <scheduler/lock.hpp>
#include <stdint.h>

constexpr uint8_t BLOCKS_PER_BUCKET = 8;

class MMap;

// TODO: figure this out
#ifdef _x86_64_
constexpr uintptr_t MEM_PHYS_OFFSET = 0xffff800000000000;
constexpr uintptr_t KERNEL_PHYS_OFFSET = 0xffffffff80000000;
#else
constexpr uintptr_t MEM_PHYS_OFFSET = 0xC0000000;
constexpr uintptr_t KERNEL_PHYS_OFFSET = 0x100000;
#endif

// Physical Memory Manager
class PMM {
public:
  static void Initialize(MMap &memoryMap);
  static void *Alloc(uintptr_t count);
  static void *AllocZ(uintptr_t count);
  static uintptr_t AllocBlock();

  static inline Bitmap &GetBitmap() { return bitmap; }

private:
  static void *InnerAlloc(uintptr_t count, uintptr_t limit);

  static Bitmap bitmap;
  static uintptr_t highestPage;
  static TicketLock s_lock;
  static uintptr_t s_lastUsedIndex;
};