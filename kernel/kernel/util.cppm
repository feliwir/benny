module;
#include <stddef.h>
#include <stdint.h>
export module util;

export void *memcpy(void *destination, const void *source, size_t num) {
  auto dst = reinterpret_cast<unsigned char *>(destination);
  auto src = reinterpret_cast<const unsigned char *>(source);

  for (size_t i = 0; i < num; ++i) {
    *dst++ = *src++;
  }

  return destination;
}

export void *memset(void *ptr, int value, size_t num) {
  auto uc_ptr = reinterpret_cast<unsigned char *>(ptr);
  for (size_t i = 0; i < num; ++i) {
    *uc_ptr++ = value;
  }

  return ptr;
}

export template <class T> inline void advancePtr(T *&ptr, const size_t bytes) {
  auto bytePtr = reinterpret_cast<uint8_t *>(ptr);
  bytePtr += bytes;
  ptr = reinterpret_cast<T *>(bytePtr);
}