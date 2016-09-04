#pragma once
#include <stdint.h>

class Vga
{
    public:
        enum Color
        {
            COLOR_BLACK = 0,
            COLOR_BLUE = 1,
            COLOR_GREEN = 2,
            COLOR_CYAN = 3,
            COLOR_RED = 4,
            COLOR_MAGENTA = 5,
            COLOR_BROWN = 6,
            COLOR_LIGHT_GREY = 7,
            COLOR_DARK_GREY = 8,
            COLOR_LIGHT_BLUE = 9,
            COLOR_LIGHT_GREEN = 10,
            COLOR_LIGHT_CYAN = 11,
            COLOR_LIGHT_RED = 12,
            COLOR_LIGHT_MAGENTA = 13,
            COLOR_LIGHT_BROWN = 14,
            COLOR_WHITE = 15,
        };

        Vga();
        ~Vga();

        void Clear();
        void Write(const char* string);
    private:
        static inline uint8_t MakeColor(const Color fg, const Color bg) 
        {
            return fg | bg << 4;
        }

        static inline uint16_t MakeEntry(const unsigned char uc, const uint8_t color) 
        {
            return (uint16_t) uc | (uint16_t) color << 8;
        }
    private:
        static const uintptr_t VGA_MEM;
        uint16_t* m_index;
};