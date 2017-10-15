
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