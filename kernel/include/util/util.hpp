#pragma once
#include <stddef.h>
#include <stdint.h>

#define K 1024
#define M (1024 * K)
#define G (1024 * M)

// Use C mangling so the compiler recognizes them and replaces them with
// builtins
extern "C" {
void *memcpy(void *destination, const void *source, size_t num);
void *memset(void *ptr, int value, size_t num);
}

template <class T> inline void advancePtr(T *&ptr, const size_t bytes) {
  auto bytePtr = reinterpret_cast<uint8_t *>(ptr);
  bytePtr += bytes;
  ptr = reinterpret_cast<T *>(bytePtr);
}