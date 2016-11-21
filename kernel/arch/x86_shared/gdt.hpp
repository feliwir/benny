#pragma once
#include <stdint.h>

class Gdt {
public:
  struct Table {
    uint16_t limit;
    uint32_t address;
  } __attribute__((packed));

  struct Entry {
    uint16_t limit1;
    uint16_t address1;
    uint8_t address2;
    union {
      uint8_t access;
      struct {
        uint8_t accessed : 1;
        uint8_t readWrite : 1;
        uint8_t dc : 1;
        uint8_t execute : 1;
        uint8_t isSystem : 1;
        uint8_t privlevel : 2;
        uint8_t present : 1;
      } accessBits;
    };
    uint8_t limit2 : 4;
    uint8_t user : 1;
    uint8_t reserved : 1;
    uint8_t is32bit : 1;
    uint8_t granularity : 1;
    uint8_t address3;
  } __attribute__((packed));

  void Initialize();

private:
  Table m_table;
  Entry *m_entries;
};