#pragma once
#include <stdint.h>
#include "base.h"

struct AuxRegs {
    uint32_t irq_status;
    uint32_t enables;
    uint32_t reserved[14];
    uint32_t mu_io;
    uint32_t mu_ier;
    uint32_t mu_iir;
    uint32_t mu_lcr;
    uint32_t mu_mcr;
    uint32_t mu_lsr;
    uint32_t mu_msr;
    uint32_t mu_scratch;
    uint32_t mu_control;
    uint32_t mu_status;
    uint32_t mu_baud_rate;
};

#define REGS_AUX ((struct AuxRegs *)(RPI_PERIPHERAL_BASE + 0x00215000))

void uart_init();
char uart_recv();
void uart_send(char c);
void uart_send_string(char *str);