#pragma once
#include <scheduler/lock.hpp>
#include <stdint.h>

class Vga final {
public:
  enum Color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
  };

  enum IntegerMode {
    IM_DEC = 10,
    IM_HEX = 16,
  };

  Vga();

  void Clear();
  void Write(const char *string);
  void PutChar(const char, const uint8_t x, const uint8_t y);
  void Scroll(uint8_t amount);
  void ClearLine();
  void ClearLine(uint8_t y);
  Vga &operator<<(const char *string);
  Vga &operator<<(const char c);
  Vga &operator<<(const int32_t num);
  Vga &operator<<(const uint32_t num);
  Vga &operator<<(const uint64_t num);

  [[nodiscard]] inline IntegerMode GetIntegerMode() const { return m_intMode; }
  inline void SetIntegerMode(IntegerMode mode) { m_intMode = mode; }

  static constexpr char endl = '\n';

private:
  static constexpr uint8_t MakeColor(const Color fg, const Color bg) {
    return fg | bg << 4;
  }

  static constexpr uint16_t MakeEntry(const unsigned char uc,
                                      const uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
  }

private:
  static constexpr uintptr_t VGA_MEM = 0xB8000;
  static constexpr uint8_t VGA_WIDTH = 80;
  static constexpr uint8_t VGA_HEIGHT = 25;
  static TicketLock s_lock;
  uint8_t m_x{0};
  uint8_t m_y{0};
  uint8_t m_color;
  IntegerMode m_intMode{IM_DEC};
};
