#pragma once
#include <stdint.h>

enum SegmentAccess : uint8_t {
  SA_NONE = 0,
  SA_ACCESSED = (1 << 0),
  SA_READABLE = (1 << 1),
  SA_WRITABLE = (1 << 1),
  SA_DIRECTION = (1 << 2),
  SA_CONFORMING = (1 << 2),
  SA_EXECUTABLE = (1 << 3),
  SA_SEGMENT = (1 << 4),
  SA_RING0 = 0,
  SA_RING1 = (1 << 5),
  SA_RING2 = (1 << 6),
  SA_RING3 = (1 << 5) | (1 << 6),
  SA_PRESENT = (1 << 7),
};

enum SegmentFlags : uint8_t {
  SF_NONE = 0,
  SF_AVAILABE = (1 << 0),
  SF_LONGMODE = (1 << 1),
  SF_USE32BIT = (1 << 2),
  SF_USE4KSIZE = (1 << 3),
};

struct SegmentDescriptor {
  uint16_t limit0;
  uint16_t base0;
  uint8_t base1;
  uint8_t access;
  uint8_t limit1 : 4;
  uint8_t flags0 : 4;
  uint8_t base2;

  SegmentDescriptor();

  SegmentDescriptor(uint32_t base, uint32_t length, uint8_t access,
                    uint8_t flags);

  void SetFlags(uint8_t flags) { this->flags0 = flags & 0x0F; }

  uint8_t Flags() const { return this->flags0; }

  uint32_t Limit() const { return this->limit0 | (this->limit1 << 16); }

  void SetLimit(uint32_t value) {
    this->limit0 = (value & 0x0FFFF) >> 0;
    this->limit1 = (value & 0xFFFFF) >> 16;
  }

  uint32_t Base() const {
    return this->base0 | (this->base1 << 16) | (this->base2 << 24);
  }

  void SetBase(uint32_t value) {
    this->base0 = (value & 0x0000FFFF) >> 0;
    this->base1 = (value & 0x00FF0000) >> 16;
    this->base2 = (value & 0xFF000000) >> 24;
  }
} __attribute__((packed));

static_assert(sizeof(SegmentDescriptor) == 8,
              "SegmentDescriptor must be 8 bytes large.");

class GDT {
public:
  static void Initialize();

  static SegmentDescriptor &Descriptor(const uint32_t index);
  static uint32_t Length();

private:
  static const uint32_t s_length = 8;
  static SegmentDescriptor s_descriptors[s_length];
  GDT() = delete;
};