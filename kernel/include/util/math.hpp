#pragma once

namespace kmath {
template <typename T> inline T div_roundup(T a, T b) {
  return (a + (b - 1)) / b;
}

template <typename T> inline T align_up(T a, T b) {
  return div_roundup(a, b) * b;
}

template <typename T> inline T align_down(T a, T b) { return (a / b) * b; }
} // namespace kmath