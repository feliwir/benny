#include <vga.hpp>

const uintptr_t Vga::VGA_MEM = 0xB8000;

Vga::Vga() : m_index(reinterpret_cast<uint16_t*>(VGA_MEM))
{

}

Vga::~Vga()
{
    m_index = nullptr;
}
