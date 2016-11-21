#pragma once
#include <stddef.h>

// Use the function declaration as it is standard in C++ (checked on
// cplusplus.org)
void *memcpy(void *destination, const void *source, size_t num);
void *memset(void *ptr, int value, size_t num);