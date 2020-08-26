#include "gdt.hpp"

extern "C" int loadGDT();

static SegmentDescriptor invalid;

SegmentDescriptor GDT::s_descriptors[GDT::s_length] = {
    SegmentDescriptor(),
#ifdef _x86_64_
    SegmentDescriptor(0x00000000, 0x00000000,
                      SA_PRESENT | SA_EXECUTABLE | SA_READABLE | SA_SEGMENT |
                          SA_RING0,
                      SF_LONGMODE),
    SegmentDescriptor(0x00000000, 0x00000000,
                      SA_PRESENT | SA_WRITABLE | SA_SEGMENT | SA_RING0, 0),
    SegmentDescriptor(0x00000000, 0x00000000,
                      SA_PRESENT | SA_EXECUTABLE | SA_READABLE | SA_SEGMENT |
                          SA_RING3,
                      SF_LONGMODE),
    SegmentDescriptor(0x00000000, 0x00000000,
                      SA_PRESENT | SA_WRITABLE | SA_SEGMENT | SA_RING3, 0),
#elif _i386_
    SegmentDescriptor(0x00000000, 0xFFFFFFFF,
                      SA_PRESENT | SA_EXECUTABLE | SA_SEGMENT | SA_RING0,
                      SF_USE4KSIZE | SF_USE32BIT),
    SegmentDescriptor(0x00000000, 0xFFFFFFFF,
                      SA_PRESENT | SA_WRITABLE | SA_SEGMENT | SA_RING0,
                      SF_USE4KSIZE | SF_USE32BIT),
    SegmentDescriptor(0x00000000, 0xFFFFFFFF,
                      SA_PRESENT | SA_EXECUTABLE | SA_SEGMENT | SA_RING3,
                      SF_USE4KSIZE | SF_USE32BIT),
    SegmentDescriptor(0x00000000, 0xFFFFFFFF,
                      SA_PRESENT | SA_WRITABLE | SA_SEGMENT | SA_RING3,
                      SF_USE4KSIZE | SF_USE32BIT),
#endif
    SegmentDescriptor(),
    SegmentDescriptor(),
    SegmentDescriptor()};

const SegmentDescriptor &GDT::Descriptor(const uint32_t index) {
  if (index >= GDT::s_length) {
    return invalid;
  }
  return GDT::s_descriptors[index];
}

void GDT::Initialize() {

  struct {
    uint16_t limit;

    void *pointer;
  } __attribute__((packed)) gdtp = {
      .limit = GDT::s_length * sizeof(SegmentDescriptor) - 1,
      .pointer = &GDT::s_descriptors,
  };

#ifdef _x86_64_
  static_assert(sizeof(gdtp) == 10, "GDTP must be 10 bytes long in Longmode.");
#elif _i386_
  static_assert(sizeof(gdtp) == 6, "GDTP must be 6 bytes long in 32-bit mode.");
#endif

  asm volatile("lgdt %0" : : "m"(gdtp));

  loadGDT();
}