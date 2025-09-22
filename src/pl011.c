#include "pl011.h"

#define UART_INT_RX (1 << 4)
#define UART_INT_TX (1 << 5)

void pl011_init()
{
    gpio_pin_set_func(TXD, GFAlt0);
    gpio_pin_set_func(RXD, GFAlt0);
    
    BCM2711_REG(PL011_CR) = (0 << 0);
    BCM2711_REG(PL011_ICR) = 0x7FF;
    
    /* Baud rate:
     * 48000000 / (16*115200) = 26.0416
     * fractional part = 0.0416 * 64 + 0.5 = 3
     */
    BCM2711_REG(PL011_IBRD) = 26;
    BCM2711_REG(PL011_FBRD) = 3;
    
    BCM2711_REG(PL011_LCRH) = LCRH_FEN | LCRH_WLEN_8BIT;
    
    BCM2711_REG(PL011_IMSC) = UART_INT_RX;
    
    BCM2711_REG(PL011_CR) = CR_UARTEN | CR_TXE | CR_RXE;
}

char pl011_recv()
{
    while (BCM2711_REG(PL011_FR) & FR_RXFE);
    return (char)(BCM2711_REG(PL011_DR) & 0xFF);
}

void pl011_sendc(char c)
{
    while (BCM2711_REG(PL011_FR) & FR_TXFF);
    BCM2711_REG(PL011_DR) = (unsigned int)c;
}

void pl011_send_string(char *str)
{
    while (*str) {
        if (*str == '\n') {
            pl011_sendc('\r');
        }
        pl011_sendc(*str++);
    }
}

void handle_uart_irq()
{
    uint32_t mis = BCM2711_REG(PL011_MIS);
    
    if (mis & UART_INT_RX) {
        char c = pl011_recv();
        pl011_sendc(c);
        BCM2711_REG(PL011_ICR) = UART_INT_RX;
    }
    
    if (mis & UART_INT_TX) {
        BCM2711_REG(PL011_ICR) = UART_INT_TX;
    }
}