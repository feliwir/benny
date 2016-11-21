#include <util.hpp>

void *memcpy(void *destination, const void *source, size_t num) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(destination);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(source);

  for (size_t i = 0; i < num; ++i) {
    dst[i] = src[i];
  }

  return destination;
}

void *memset(void *ptr, int value, size_t num) {
  unsigned char *uc_ptr = reinterpret_cast<unsigned char *>(ptr);
  for (size_t i = 0; i < num; ++i) {
    uc_ptr[i] = value;
  }

  return ptr;
}