#include "keyboard.hpp"
#include "io.hpp"
#include "pic.hpp"
#include "vga.hpp"

Vga screen;

__attribute__((interrupt)) void KeyboardInterrupt(__attribute__((unused))
                                                  InterruptFrame *frame) {
  // Read the scancode from the PS/2 bus
  uint8_t scancode = inb(0x60);
  Keyboard::Receive(scancode);
  PIC::NotifyEndOfInterrupt((int)HardwareInterrupts::Keyboard);
}

void Keyboard::Receive(uint8_t scancode) { screen << scancode; }