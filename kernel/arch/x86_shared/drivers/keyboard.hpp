#pragma once
#include "../util/interrupts.hpp"

__attribute__((interrupt)) void KeyboardInterrupt(InterruptFrame *frame);

class Keyboard {
public:
  static void Receive(uint8_t scancode);
};