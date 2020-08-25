#pragma once
#include "interrupts.hpp"

__attribute__((interrupt)) void TimerInterrupt(InterruptFrame *frame);

class Timer {
public:
  static void Update();
};