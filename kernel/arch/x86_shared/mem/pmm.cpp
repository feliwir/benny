#include "pmm.hpp"
#include "vga.hpp"
import util;
import mmap;

uint8_t* PMM::bitmap = (uint8_t *)(&end);
uint32_t PMM::totalBlocks;
uint32_t PMM::bitmapSize;
uint8_t *PMM::memStart;

uintptr_t memSize;

MMap memoryMap;
Vga term;

void processMultiboot(multiboot_tag *tag) {
  uint32_t total_size = tag->type;
  term << "Total tags size: " << total_size << term.endl;
  advancePtr(tag, MULTIBOOT_TAG_ALIGN);

  term << (int)sizeof(multiboot_tag) << term.endl;

  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_MMAP: // Memory map
    {
      auto *tag_mmap = reinterpret_cast<multiboot_tag_mmap *>(tag);
      term << "Found memory map tag!" << term.endl;
      memoryMap.Initialize(tag_mmap);
    } break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
      auto *tag_meminfo = reinterpret_cast<multiboot_tag_basic_meminfo *>(tag);
      memSize = tag_meminfo->mem_upper;
      term << "Found basic memory info!" << term.endl;
      break;
    }
    auto next =
        ((tag->size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1));
    advancePtr(tag, next);
  }

  term << "Done processing tags!" << term.endl;
}

void PMM::Initialize(multiboot_tag* bootInfo) {
  term.Clear();
  processMultiboot(bootInfo);

  memoryMap.Print();

  totalBlocks = 8096 / BLOCK_SIZE;

  // For the given memory size, how many bytes is needed for the bitmap?
  // (mem_size nees to be multiple of BLOCK_SIZE = 4096)
  bitmapSize = totalBlocks / BLOCKS_PER_BUCKET;
  if (bitmapSize * BLOCKS_PER_BUCKET < totalBlocks)
    bitmapSize++;

  // Clear bitmap
  memset(bitmap, 0, bitmapSize);

  // Start of all blcoks
  memStart = (uint8_t *)BLOCK_ALIGN(((uintptr_t)(bitmap + bitmapSize)));
}

uint32_t PMM::AllocateBlock() {
  uint32_t free_block = FirstFreeBlock();
  SETBIT(free_block);
  return free_block;
}

void PMM::FreeBlock(uint32_t blockNumber) { CLEARBIT(blockNumber); }

uint32_t PMM::FirstFreeBlock() {
  uint32_t i;
  for (i = 0; i < totalBlocks; i++) {
    if (!ISSET(i))
      return i;
  }
  return (uint32_t)-1;
}