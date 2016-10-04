#include <vga.hpp>
#include <util.hpp>

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

void Vga::ClearLine() {
  ClearLine(m_y);
}

void Vga::ClearLine(uint8_t y) {
  uint16_t *buffer = reinterpret_cast<uint16_t *>(VGA_MEM);
  for (uint8_t x = 0; x < VGA_WIDTH; ++x) {
    buffer[y * VGA_WIDTH + x] = MakeEntry(' ', m_color);
  }
}

Vga &Vga::operator<<(const char *string) {
  Write(string);
  return *this;
}

void Vga::Scroll(uint8_t amount) {
  uint16_t *buffer = reinterpret_cast<uint16_t *>(VGA_MEM);
  if (amount < VGA_HEIGHT) {
    memcpy(buffer, &buffer[amount * VGA_WIDTH], (VGA_HEIGHT - amount + 1) * VGA_WIDTH * sizeof(uint16_t));
    for (int y = VGA_HEIGHT - 1; y > VGA_HEIGHT - amount - 1; --y) {
      ClearLine(y);
    }
  } else {
    Clear();
  }
  m_y -= amount;
  if (m_y < 0) {
    m_y = 0;
  }
}

void Vga::Write(const char *string) {
  if (m_y >= VGA_HEIGHT) {
    Scroll(m_y - VGA_HEIGHT + 1);
  }
  while (*string) {
    if (*string == '\n') {
      ++m_y;
      if (m_y == VGA_HEIGHT) { // This should never be > VGA_HEIGHT because of the previous check
        Scroll(1);
      }
      m_x = 0;
    } else {
      PutChar(*string, m_x, m_y);
      ++m_x;
    }
    ++string;
  }
}
