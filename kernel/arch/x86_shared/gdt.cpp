#include "gdt.hpp"

static SegmentDescriptor invalid;

// Class members
SegmentDescriptor GDT::s_descriptors[GDT::s_length];

SegmentDescriptor& GDT::Descriptor(const uint32_t index) {
  if (index >= GDT::s_length) {
    return invalid;
  }
  return GDT::s_descriptors[index];
}

SegmentDescriptor::SegmentDescriptor() : limit0(0), base0(0), 
base1(0), access(SA_NONE), limit1(0),
flags0(0), base2(0) 
{}

SegmentDescriptor::SegmentDescriptor(uint32_t base,uint32_t length, uint8_t access,uint8_t flags)
: access(access) 
{
    SetBase(base);
    SetFlags(flags);

    if (Flags() & SF_USE4KSIZE) 
    {
      SetLimit(length >> 12);
    } 
    else 
    {
      SetLimit(length);
    }
}

void GDT::Initialize() 
{
  // Null descriptor
  GDT::s_descriptors[0] = SegmentDescriptor();
  GDT::s_descriptors[1] = SegmentDescriptor(0x00000000, 0xFFFFFFFF, 
                        SA_PRESENT | SA_EXECUTABLE | SA_SEGMENT | SA_RING0,
                        SF_USE4KSIZE | SF_USE32BIT);
  GDT::s_descriptors[2] = SegmentDescriptor(0x00000000, 0xFFFFFFFF, 
                        SA_PRESENT | SA_WRITABLE | SA_SEGMENT | SA_RING0,
                        SF_USE4KSIZE | SF_USE32BIT);
  GDT::s_descriptors[3] = SegmentDescriptor(0x00000000, 0xFFFFFFFF,
                        SA_PRESENT | SA_EXECUTABLE | SA_SEGMENT | SA_RING3,
                        SF_USE4KSIZE | SF_USE32BIT);
  GDT::s_descriptors[4] = SegmentDescriptor(0x00000000, 0xFFFFFFFF, 
                        SA_PRESENT | SA_WRITABLE | SA_SEGMENT | SA_RING3,
                        SF_USE4KSIZE | SF_USE32BIT);

  struct{
    uint16_t limit;
    void *pointer;
  } __attribute__((packed)) gdtp = 
  {
      .limit = GDT::s_length * sizeof(SegmentDescriptor) - 1,
      .pointer = &GDT::s_descriptors,
  };

  asm volatile("lgdt %0" : : "m"(gdtp));

  asm volatile("mov $0x10, %ax");
  asm volatile("mov %ax, %ds");
  asm volatile("mov %ax, %es");
  asm volatile("mov %ax, %fs");
  asm volatile("mov %ax, %gs");
  asm volatile("mov %ax, %ss");
}