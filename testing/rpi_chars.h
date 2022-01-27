#ifndef _RPI_CHARS_H_
#define _RPI_CHARS_H_

#include <stdint.h>

typedef uint8_t rpi_char_t[8]; // 8 byte array

// hecho a manopla con https://maxpromer.github.io/LCD-Character-Creator/

void rpi_putchar(char c, uint8_t y, uint8_t x);


const rpi_char_t UNKNOWN_ = {
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F,
    0x1F
};

const rpi_char_t unknown_ = {
    0x1C,
    0x1C,
    0x1C,
    0x1C,
    0x1C,
    0x00,
    0x00,
    0x00
};

// 5 x 3

const rpi_char_t ascii_0_z[] = {
    {
        0x08,
        0x14,
        0x14,
        0x14,
        0x08,
        0x00,
        0x00,
        0x00
    }, // 0
    {
        0x08,
        0x18,
        0x08,
        0x08,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // 1
    {
        0x1C,
        0x14,
        0x04,
        0x08,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // 2
    {
        0x18,
        0x04,
        0x08,
        0x04,
        0x18,
        0x00,
        0x00,
        0x00
    }, // 3
    {
        0x14,
        0x14,
        0x1C,
        0x04,
        0x04,
        0x00,
        0x00,
        0x00
    }, // 4
    {
        0x1C,
        0x10,
        0x1C,
        0x04,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // 5
    {
        0x1C,
        0x10,
        0x1C,
        0x14,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // 6
    {
        0x1C,
        0x04,
        0x04,
        0x04,
        0x04,
        0x00,
        0x00,
        0x00
    }, // 7
    {
        0x1C,
        0x14,
        0x1C,
        0x14,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // 8
    {
        0x1C,
        0x14,
        0x1C,
        0x04,
        0x04,
        0x00,
        0x00,
        0x00
    }, // 9
    {
        0x00,
        0x08,
        0x00,
        0x08,
        0x00,
        0x00,
        0x00,
        0x00
    }, // :
    {
        0x00,
        0x08,
        0x00,
        0x08,
        0x08,
        0x00,
        0x00,
        0x00
    }, // ;
    {
        0x04,
        0x08,
        0x10,
        0x08,
        0x04,
        0x00,
        0x00,
        0x00
    }, // <
    {
        0x00,
        0x1C,
        0x00,
        0x1C,
        0x00,
        0x00,
        0x00,
        0x00
    }, // =
    {
        0x10,
        0x08,
        0x04,
        0x08,
        0x10,
        0x00,
        0x00,
        0x00
    }, // >
    {
        0x1C,
        0x14,
        0x04,
        0x08,
        0x08,
        0x00,
        0x00,
        0x00
    }, // ?
    {
        0x1C,
        0x1C,
        0x14,
        0x1C,
        0x18,
        0x00,
        0x00,
        0x00
    }, // @
    {
        0x08,
        0x14,
        0x1C,
        0x14,
        0x14,
        0x00,
        0x00,
        0x00
    }, // A
    {
        0x18,
        0x14,
        0x18,
        0x14,
        0x18,
        0x00,
        0x00,
        0x00
    }, // B
    {
        0x0C,
        0x10,
        0x10,
        0x10,
        0x0C,
        0x00,
        0x00,
        0x00
    }, // C
    {
        0x18,
        0x14,
        0x14,
        0x14,
        0x18,
        0x00,
        0x00,
        0x00
    }, // D
    {
        0x1C,
        0x10,
        0x1C,
        0x10,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // E
    {
        0x1C,
        0x10,
        0x1C,
        0x10,
        0x10,
        0x00,
        0x00,
        0x00
    }, // F
    {
        0x0C,
        0x10,
        0x14,
        0x14,
        0x0C,
        0x00,
        0x00,
        0x00
    }, // G
    {
        0x14,
        0x14,
        0x1C,
        0x14,
        0x14,
        0x00,
        0x00,
        0x00
    }, // H
    {
        0x1C,
        0x08,
        0x08,
        0x08,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // I
    {
        0x04,
        0x04,
        0x04,
        0x14,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // J
    {
        0x14,
        0x14,
        0x18,
        0x14,
        0x14,
        0x00,
        0x00,
        0x00
    }, // K
    {
        0x10,
        0x10,
        0x10,
        0x10,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // L
    {
        0x14,
        0x1C,
        0x14,
        0x14,
        0x14,
        0x00,
        0x00,
        0x00
    }, // M
    {
        0x00,
        0x18,
        0x14,
        0x14,
        0x14,
        0x00,
        0x00,
        0x00
    }, // N
    {
        0x1C,
        0x14,
        0x14,
        0x14,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // O
    {
        0x1C,
        0x14,
        0x1C,
        0x10,
        0x10,
        0x00,
        0x00,
        0x00
    }, // P
    {
        0x1C,
        0x14,
        0x14,
        0x1C,
        0x04,
        0x00,
        0x00,
        0x00
    }, // Q
    {
        0x1C,
        0x14,
        0x18,
        0x14,
        0x14,
        0x00,
        0x00,
        0x00
    }, // R
    {
        0x0C,
        0x10,
        0x08,
        0x04,
        0x18,
        0x00,
        0x00,
        0x00
    }, // S
    {
        0x1C,
        0x08,
        0x08,
        0x08,
        0x08,
        0x00,
        0x00,
        0x00
    }, // T
    {
        0x14,
        0x14,
        0x14,
        0x14,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // U
    {
        0x14,
        0x14,
        0x14,
        0x08,
        0x08,
        0x00,
        0x00,
        0x00
    }, // V
    {
        0x00,
        0x14,
        0x1C,
        0x1C,
        0x14,
        0x00,
        0x00,
        0x00
    }, // W
    {
        0x00,
        0x14,
        0x08,
        0x08,
        0x14,
        0x00,
        0x00,
        0x00
    }, // X
    {
        0x00,
        0x14,
        0x08,
        0x08,
        0x08,
        0x00,
        0x00,
        0x00
    }, // Y
    {
        0x1C,
        0x04,
        0x08,
        0x10,
        0x1C,
        0x00,
        0x00,
        0x00
    }, // Z
};

// 8 x 5

/* const rpi_char_t ASCII_0_Z[] = {
    {
        0x0C,
        0x12,
        0x12,
        0x12,
        0x12,
        0x12,
        0x0C,
        0x00
    }, // 0
    {
        0x04,
        0x0C,
        0x04,
        0x04,
        0x04,
        0x04,
        0x0E,
        0x00
    }, // 1
    {
        0x0C,
        0x12,
        0x02,
        0x04,
        0x08,
        0x10,
        0x1E,
        0x00
    }, // 2
    {
        0x0C,
        0x12,
        0x02,
        0x04,
        0x02,
        0x12,
        0x0C,
        0x00
    }, // 3
    {
        0x12,
        0x12,
        0x12,
        0x1E,
        0x02,
        0x02,
        0x02,
        0x00
    }, // 4
    {
        0x1E,
        0x10,
        0x10,
        0x1C,
        0x02,
        0x02,
        0x1C,
        0x00
    }, // 5
    {
        0x0C,
        0x10,
        0x10,
        0x1C,
        0x12,
        0x12,
        0x0C,
        0x00
    }, // 6
    {
        0x1E,
        0x12,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x00
    }, // 7
    {
        0x0C,
        0x12,
        0x12,
        0x0C,
        0x12,
        0x12,
        0x0C,
        0x00
    }, // 8
    {
        0x0C,
        0x12,
        0x12,
        0x0E,
        0x02,
        0x02,
        0x0C,
        0x00
    }, // 9
    {
        0x00,
        0x08,
        0x00,
        0x00,
        0x00,
        0x08,
        0x00,
        0x00
    }, // :
    {
        0x00,
        0x08,
        0x00,
        0x00,
        0x00,
        0x04,
        0x08,
        0x00
    }, // ;
    {
        0x02,
        0x04,
        0x08,
        0x10,
        0x08,
        0x04,
        0x02,
        0x00
    }, // <
    {
        0x00,
        0x00,
        0x00,
        0x1E,
        0x00,
        0x1E,
        0x00,
        0x00
    }, // =
    {
        0x10,
        0x08,
        0x04,
        0x02,
        0x04,
        0x08,
        0x10,
        0x00
    }, // >
    {
        0x0E,
        0x11,
        0x01,
        0x02,
        0x04,
        0x04,
        0x00,
        0x04
    }, // ?
    {
        0x0E,
        0x12,
        0x16,
        0x16,
        0x16,
        0x10,
        0x0E,
        0x00
    }, // @
    {
        0x0C,
        0x12,
        0x12,
        0x1E,
        0x12,
        0x12,
        0x12,
        0x00
    }, // A
    {
        0x1C,
        0x12,
        0x12,
        0x1C,
        0x12,
        0x12,
        0x1C,
        0x00
    }, // B
    {
        0x0E,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x0E,
        0x00
    }, // C
    {
        0x1C,
        0x12,
        0x12,
        0x12,
        0x12,
        0x12,
        0x1C,
        0x00
    }, // D
    {
        0x1E,
        0x10,
        0x10,
        0x1E,
        0x10,
        0x10,
        0x1E,
        0x00
    }, // E
    {
        0x1E,
        0x10,
        0x10,
        0x1E,
        0x10,
        0x10,
        0x10,
        0x00
    }, // F
    {
        0x0E,
        0x10,
        0x10,
        0x16,
        0x12,
        0x12,
        0x0E,
        0x00
    }, // G
    {
        0x12,
        0x12,
        0x12,
        0x1E,
        0x12,
        0x12,
        0x12,
        0x00
    }, // H
    {
        0x0E,
        0x04,
        0x04,
        0x04,
        0x04,
        0x04,
        0x0E,
        0x00
    }, // I
    {
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x12,
        0x0C,
        0x00
    }, // J
    {
        0x00,
        0x12,
        0x14,
        0x18,
        0x18,
        0x14,
        0x12,
        0x00
    }, // K
    {
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x10,
        0x1E,
        0x00
    }, // L
    {
        0x00,
        0x11,
        0x1B,
        0x15,
        0x11,
        0x11,
        0x11,
        0x00
    }, // M
    {
        0x00,
        0x12,
        0x1A,
        0x1A,
        0x16,
        0x16,
        0x12,
        0x00
    }, // N
    {
        0x00,
        0x1E,
        0x12,
        0x12,
        0x12,
        0x12,
        0x1E,
        0x00
    }, // O
    {
        0x00,
        0x1C,
        0x12,
        0x12,
        0x1C,
        0x10,
        0x10,
        0x00
    }, // P
    {
        0x1E,
        0x12,
        0x12,
        0x12,
        0x12,
        0x1E,
        0x04,
        0x02
    }, // Q
    {
        0x00,
        0x1C,
        0x12,
        0x12,
        0x1C,
        0x14,
        0x12,
        0x00
    }, // R
    {
        0x0E,
        0x10,
        0x10,
        0x0C,
        0x02,
        0x02,
        0x1C,
        0x00
    }, // S
    {
        0x1F,
        0x04,
        0x04,
        0x04,
        0x04,
        0x04,
        0x04,
        0x00
    }, // T
    {
        0x00,
        0x12,
        0x12,
        0x12,
        0x12,
        0x12,
        0x0C,
        0x00
    }, // U
    {
        0x00,
        0x11,
        0x11,
        0x0A,
        0x0A,
        0x04,
        0x04,
        0x00
    }, // V
    {
        0x00,
        0x00,
        0x11,
        0x15,
        0x15,
        0x0A,
        0x0A,
        0x00
    }, // W
    {
        0x11,
        0x11,
        0x0A,
        0x04,
        0x0A,
        0x11,
        0x11,
        0x00
    }, // X
    {
        0x11,
        0x11,
        0x0A,
        0x04,
        0x04,
        0x04,
        0x04,
        0x00
    }, // Y
    {
        0x1F,
        0x01,
        0x02,
        0x04,
        0x08,
        0x10,
        0x1F,
        0x00
    }, // Z
}; */


#endif