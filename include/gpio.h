#pragma once
#include <stdint.h>
#include "base.h"
struct GpioPinData {
    uint32_t reserved;
    uint32_t data[2];
};

struct GpioRegs {
    uint32_t func_select[6];
    struct GpioPinData output_set;
    struct GpioPinData output_clear;
    struct GpioPinData level;
    struct GpioPinData ev_detect_status;
    struct GpioPinData re_detect_enable;
    struct GpioPinData fe_detect_enable;
    struct GpioPinData hi_detect_enable;
    struct GpioPinData lo_detect_enable;
    struct GpioPinData async_re_detect;
    struct GpioPinData async_fe_detect;
    uint32_t reserved;
    uint32_t pupd_enable;
    uint32_t pupd_enable_clocks[2];
};

#define REGS_GPIO ((struct GpioRegs *)(RPI_PERIPHERAL_BASE + 0x00200000))

typedef enum _GpioFunc {
    GFInput = 0,
    GFOutput = 1,
    GFAlt0 = 4,
    GFAlt1 = 5,
    GFAlt2 = 6,
    GFAlt3 = 7,
    GFAlt4 = 3,
    GFAlt5 = 2
} GpioFunc;

void gpio_pin_set_func(uint8_t pinNumber, GpioFunc func);

void gpio_pin_enable(uint8_t pinNumber);