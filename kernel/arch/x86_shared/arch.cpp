#include "gdt.hpp"
#include "pic.hpp"
#include <arch.hpp>
#include <vga.hpp>

void IntHandler_DivByZero()
{
    Vga screen;
    screen.Clear();
    screen << "Divison by zero! Panic!";
    while(true)
    {}
}

void Arch::Initialize() {
  GDT::Initialize();
  PIC::AddHandler(0,&IntHandler_DivByZero,0x08,GT_32bitInt);
  PIC::Initialize();
}