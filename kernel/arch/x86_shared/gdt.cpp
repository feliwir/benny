#include "gdt.hpp"

static SegmentDescriptor invalid;

SegmentDescriptor &GDT::Descriptor(const uint32_t index) {
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
}