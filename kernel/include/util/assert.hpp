#pragma once
#include <vga.hpp>

void AssertFunc(bool condition, const char *file, int line) {
  if (!condition) {
    Vga terminal;
    terminal.Clear();
    terminal << "Assert failed! In File: " << file << ":" << line;
    while (true) {
    }
  }
}

#define assert(COND) AssertFunc(COND, __FILE__, __LINE__)