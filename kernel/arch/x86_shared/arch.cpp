#include "gdt.hpp"
#include <arch.hpp>
Gdt gdt;

void Arch::Initialize() { gdt.Initialize(); }