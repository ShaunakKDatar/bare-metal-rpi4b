#pragma once
#include <stdint.h>
#include "base.h"

// GPIO Register Offsets
#define GPFSEL0    (RPI_PERIPHERAL_BASE + 0x00200000)
#define GPSET0     (RPI_PERIPHERAL_BASE + 0x0020001C)
#define GPCLR0     (RPI_PERIPHERAL_BASE + 0x00200028)

// Alternate Function enum
typedef enum {
    ALT_FN_INPUT  = 0b000,
    ALT_FN_OUTPUT = 0b001,
    ALT_FN_0      = 0b100,
    ALT_FN_1      = 0b101,
    ALT_FN_2      = 0b110,
    ALT_FN_3      = 0b111,
    ALT_FN_4      = 0b011,
    ALT_FN_5      = 0b010
} ALT_F;


int gpio_select(int gpio_number, ALT_F alt);
int gpio_enable(int gpio_number);