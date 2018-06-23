#pragma once
#include <stddef.h>

size_t strlen(const char *str);
char *itoa(int value, char *str, int base);
char *ltoa(long value, char *str, int base);
char *utoa(unsigned int value, char *str, int base);
char *ultoa(unsigned long value, char *str, int base);