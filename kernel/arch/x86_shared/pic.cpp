
#include "pic.hpp"

InterruptDescriptor PIC::s_descriptors[PIC::s_length];

void PIC::Initialize() 
{
    struct {
        uint16_t limit;
        void *pointer;
      } __attribute__((packed)) idtp = {
          .limit = PIC::s_length * sizeof(InterruptDescriptor) - 1,
          .pointer = &PIC::s_descriptors,
      };

    asm volatile("lidt %0" : : "m"(idtp));
}

void PIC::AddHandler(int index, void(*func)(),int sel,GateType g)
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(func);  
    auto& d = PIC::s_descriptors[index];
    d.offset_1 = (addr & 0xFFFF);
    d.offset_2 = (addr >> 16) & 0xFFFF;
    d.selector = sel;
    d.type  = g;
    d.used = 1;
    d.dpl = 0;

    //other fields
    d.reserved = 0;
    d.ist_index = 0;
}