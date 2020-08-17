module;
#include <stddef.h>
#include <stdint.h>
export module util;

export void *memcpy(void *destination, const void *source, size_t num) {
  unsigned char *dst = reinterpret_cast<unsigned char *>(destination);
  const unsigned char *src = reinterpret_cast<const unsigned char *>(source);

  for (size_t i = 0; i < num; ++i) {
    *dst++ = *src++;
  }

  return destination;
}

export void *memset(void *ptr, int value, size_t num) {
  unsigned char *uc_ptr = reinterpret_cast<unsigned char *>(ptr);
  for (size_t i = 0; i < num; ++i) {
    *uc_ptr++ = value;
  }

  return ptr;
}

export template <class T> 
inline void advancePtr(T *&ptr, const size_t bytes) {
  uint8_t *bytePtr = reinterpret_cast<uint8_t *>(ptr);
  bytePtr += bytes;
  ptr = reinterpret_cast<T *>(bytePtr);
}