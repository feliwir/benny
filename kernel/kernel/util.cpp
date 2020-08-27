#include <util/util.hpp>

void *memcpy(void *destination, const void *source, size_t num) {
  auto dst = reinterpret_cast<unsigned char *>(destination);
  auto src = reinterpret_cast<const unsigned char *>(source);

  for (size_t i = 0; i < num; ++i) {
    *dst++ = *src++;
  }

  return destination;
}

void *memset(void *ptr, int value, size_t num) {
  auto uc_ptr = reinterpret_cast<unsigned char *>(ptr);
  for (size_t i = 0; i < num; ++i) {
    *uc_ptr++ = value;
  }

  return ptr;
}
