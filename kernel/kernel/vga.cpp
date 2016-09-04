#include <vga.hpp>

Vga::Vga()
    : m_x(0), m_y(0), m_color(MakeColor(COLOR_LIGHT_GREY, COLOR_BLACK)) {}

Vga::~Vga() {}

void Vga::Clear() {
  uint16_t *buffer = reinterpret_cast<uint16_t *>(VGA_MEM);

  for (uint8_t y = 0; y < VGA_HEIGHT; ++y) {
    for (uint8_t x = 0; x < VGA_WIDTH; ++x) {
      const uint16_t index = y * VGA_WIDTH + x;
      buffer[index] = MakeEntry(' ', m_color);
    }
  }
}

void Vga::PutChar(const char c, const uint8_t x, const uint8_t y) {
  const uint16_t index = y * VGA_WIDTH + x;
  uint16_t *buffer = reinterpret_cast<uint16_t *>(VGA_MEM);
  buffer[index] = MakeEntry(c, m_color);
}

void Vga::Write(const char *string) {
  while (*string) {
    PutChar(*string, m_x, m_y);
    ++string;
  }
}