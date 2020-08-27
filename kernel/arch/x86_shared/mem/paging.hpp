#pragma once
#include <stdint.h>

constexpr uint16_t PAGE_SIZE = 4096;

// Alignment related macro
#define IS_ALIGN(addr) ((((uint32_t)(addr)) | 0xFFFFF000) == 0)
#define PAGE_ALIGN(addr) ((((uint32_t)(addr)) & 0xFFFFF000) + 0x1000)

// Defone some address calculation macro
#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >> 12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

// Paging register manipulation macro
#define SET_PGBIT(cr0) (cr0 = cr0 | 0x80000000)
#define CLEAR_PSEBIT(cr4) (cr4 = cr4 & 0xffffffef)

// Err code interpretation
#define ERR_PRESENT 0x1
#define ERR_RW 0x2
#define ERR_USER 0x4
#define ERR_RESERVED 0x8
#define ERR_INST 0x10

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
PageDirectory kpage_dir;

class Paging {
public:
  void Initialize();

  void *virtual2phys(PageDirectory *dir, void *virtual_addr);
  void *dumb_kmalloc(uint32_t size, int align);
  void allocate_region(PageDirectory *dir, uint32_t start_va, uint32_t end_va,
                       int iden_map, int is_kernel, int is_writable);

  void allocate_page(PageDirectory *dir, uint32_t virtual_addr, uint32_t frame,
                     int is_kernel, int is_writable);

  void free_region(PageDirectory *dir, uint32_t start_va, uint32_t end_va,
                   int free);

  void free_page(PageDirectory *dir, uint32_t virtual_addr, int free);

  void switch_page_directory(PageDirectory *page_dir, uint32_t phys);

  void enable_paging();

  void *ksbrk(int size);

  void copy_page_directory(PageDirectory *dst, PageDirectory *src);

  PageTable *copy_page_table(PageDirectory *src_page_dir,
                             PageDirectory *dst_page_dir, uint32_t page_dir_idx,
                             PageTable *src);

  private:
  uint8_t * tempMem;
};
