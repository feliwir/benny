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

  // When set, the D bit (SF_USE32BIT) must be cleared.
  // Must be 0 when not in IA-32e mode or for non-code segments
  SF_LONGMODE = (1 << 1),

  // Default size; set for 32 bit, unset for 16 bit
  SF_USE32BIT = (1 << 2),

  // Granularity
  SF_USE4KSIZE = (1 << 3),
};

constexpr const uint16_t CODE_SELECTOR = 0x08;
constexpr const uint16_t DATA_SELECTOR = 0x10;
constexpr const uint16_t LONG_SELECTOR = 0x18;
constexpr const uint16_t USER_CODE_SELECTOR = 0x20;
constexpr const uint16_t USER_DATA_SELECTOR = 0x28;
constexpr const uint16_t TSS_SELECTOR = 0x30;

constexpr const uint16_t SEG_LDT = 0x2;            ///< LDT
constexpr const uint16_t SEG_TSS_AVAILABLE = 0x9;  ///< 64 bits TSS (Available)
constexpr const uint16_t SEG_TSS_BUSY = 0xB;       ///< 64 bits TSS (Busy)
constexpr const uint16_t SEG_CALL_GATE = 0xC;      ///< 64 bits Call Gate
constexpr const uint16_t SEG_INTERRUPT_GATE = 0xE; ///< 64 bits Interrupt Gate
constexpr const uint16_t SEG_TRAP_GATE = 0xE;      ///< 64 bits Trap  Gate

struct SegmentDescriptor {
  uint16_t limit0{0};
  uint16_t base0{0};
  uint8_t base1{0};
  uint8_t access{SA_NONE};
  uint8_t limit1 : 4 {0};
  uint8_t flags0 : 4 {0};
  uint8_t base2{0};

  constexpr SegmentDescriptor() = default;

  constexpr SegmentDescriptor(uint32_t base, uint32_t length, uint8_t acc,
                              uint8_t flags)
      : limit0((((flags & SF_USE4KSIZE) ? length >> 12 : length) & 0x0FFFF) >>
               0),
        base0((base & 0x0000FFFF) >> 0), base1((base & 0x00FF0000) >> 16),
        access(acc),
        limit1((((flags & SF_USE4KSIZE) ? length >> 12 : length) & 0xFFFFF) >>
               16),
        flags0(flags & 0x0F), base2((base & 0xFF000000) >> 24) {}
} __attribute__((packed));

static_assert(sizeof(SegmentDescriptor) == 8,
              "SegmentDescriptor must be 8 bytes large.");

class GDT {
public:
  GDT() = delete;
  static void Initialize();

  static const SegmentDescriptor &Descriptor(const uint32_t index);
  static uint32_t Length();

private:
  static const uint32_t s_length = 8;
  static SegmentDescriptor s_descriptors[s_length];
};