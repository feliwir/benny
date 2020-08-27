#pragma once
#include <stdint.h>
#include <multiboot.h>

constexpr uint16_t BLOCK_SIZE = 4096;
constexpr uint8_t BLOCKS_PER_BUCKET = 8;

// Define some bit manipulating operations
#define SETBIT(i)                                                              \
  bitmap[i / BLOCKS_PER_BUCKET] =                                              \
      bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEARBIT(i)                                                            \
  bitmap[i / BLOCKS_PER_BUCKET] =                                              \
      bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define ISSET(i)                                                               \
  ((bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t *)&bitmap[i / 32]))

#define BLOCK_ALIGN(addr) (((addr)&0xFFFFF000) + 0x1000)

// Defined in linker.ld, indicate the end of kernel code/data
extern uint32_t end;

// Physical Memory Manager
class PMM {
public:
  static void Initialize(multiboot_tag* bootInfo);
  static uint32_t AllocateBlock();
  static void FreeBlock(uint32_t blockNumber);
  static uint32_t FirstFreeBlock();

  static uint32_t bitmapSize;
  static uint8_t *bitmap;

private:
  static uint32_t totalBlocks;
  static uint8_t *memStart;
};