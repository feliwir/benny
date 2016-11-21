#pragma once
#include <vga.hpp>

void assert_func(bool condition, const char *file, int line) {
  if (!condition) {
    Vga terminal;
    terminal.Clear();
    terminal << "Assert failed! In File: " << file << ":" << line;
    while (true) {
    }
  }
}

#define assert(COND) assert_func(COND, __FILE__, __LINE__)