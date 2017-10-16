#include "gdt.hpp"

extern "C" void loadGDT();

static SegmentDescriptor invalid;

SegmentDescriptor GDT::s_descriptors[GDT::s_length] = {
    SegmentDescriptor(),
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

  asm volatile("lgdt %0" : : "m"(gdtp));

  loadGDT();
}