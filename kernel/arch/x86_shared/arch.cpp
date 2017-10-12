#include "gdt.hpp"
#include <arch.hpp>

void Arch::Initialize() 
{ 
    GDT::Initialize(); 
}