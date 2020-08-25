#pragma once

static bool interrupts_disabled = true;

inline void disable_interrupts() {
  // disable interrupts
  if (interrupts_disabled == false) {
    asm volatile("cli");
    interrupts_disabled = true;
  }
}

inline void enable_interrupts() {
  // enable interrupts
  if (interrupts_disabled == true) {
    asm volatile("sti");
    interrupts_disabled = false;
  }
}

template <class T, typename... F>
inline void without_interrupts(T lambda, F &&... args) {

  disable_interrupts();
  lambda(args...);
  enable_interrupts();
}