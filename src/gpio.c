#include "gpio.h"

int gpio_select(int gpio_number, ALT_F alt) {
    if (gpio_number < 0 || gpio_number > 53)
        return -1;

    int reg_index = gpio_number / 10;
    int shift = (gpio_number % 10) * 3;

    uint64_t gpfsel = GPFSEL0 + (reg_index * 4);

    uint32_t val = BCM2711_REG(gpfsel);
    val &= ~(0b111 << shift);        // Clear old function
    val |= ((alt & 0b111) << shift); // Set new function
    BCM2711_REG(gpfsel) = val;

    return 0;
}

int gpio_enable(int gpio_number) {
    if (gpio_number < 0 || gpio_number > 53)
        return -1;

    uint64_t gpset = GPSET0 + ((gpio_number / 32) * 4);
    BCM2711_REG(gpset) = (1 << (gpio_number % 32));

    return 0;
}
