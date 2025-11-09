#pragma once

#define RPI_PERIPHERAL_BASE 0xFE000000

#ifndef __ASSEMBLER__

#include <stdint.h>

#define BCM2711_REG(addr)   (*(volatile uint32_t *)(uintptr_t)(addr))
#define BCM2711_BIT(n)      (1 << n)

#endif