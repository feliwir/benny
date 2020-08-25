#include "timer.hpp"
#include "pic.hpp"

__attribute__((interrupt)) void TimerInterrupt(__attribute__((unused))
                                               InterruptFrame *frame) {

  Timer::Update();
  PIC::NotifyEndOfInterrupt((int)HardwareInterrupts::Timer);
}

void Timer::Update() {}