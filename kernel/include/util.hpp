#pragma once
#include <stddef.h>
#include <stdint.h>

// Use the function declaration as it is standard in C++ (checked on
// cplusplus.org)
void *memcpy(void *destination, const void *source, size_t num);
void *memset(void *ptr, int value, size_t num);

template <class T> inline void advancePtr(T *&ptr, const size_t bytes) {
  uint8_t *bytePtr = reinterpret_cast<uint8_t *>(ptr);
  bytePtr += bytes;
  ptr = reinterpret_cast<T *>(bytePtr);
}