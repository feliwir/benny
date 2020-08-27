#pragma once
#include <stdint.h>

// An array of bits that we use to determine of a page is used or not
class Bitmap {
public:
  inline void SetData(uint8_t *data) { m_data = data; }

  inline bool IsSet(uintptr_t bit) {
    return m_data[bit / 8] & (1 << (bit % 8));
  }

  inline void Set(uintptr_t bit) { m_data[bit / 8] |= 1 << (bit % 8); }

  inline void Unset(uintptr_t bit) { m_data[bit / 8] |= 0 << (bit % 8); }

private:
  uint8_t *m_data;
};