#include "base.h"
#include "gpio.h"

#define PL011_BASE (RPI_PERIPHERAL_BASE + 0x00201000)

#define PL011_DR        (PL011_BASE + 0x00)
#define PL011_RSRECR    (PL011_BASE + 0x04)
#define PL011_FR        (PL011_BASE + 0x18)
#define PL011_ILPR      (PL011_BASE + 0x20)
#define PL011_IBRD      (PL011_BASE + 0x24)
#define PL011_FBRD      (PL011_BASE + 0x28)
#define PL011_LCRH      (PL011_BASE + 0x2C)
#define PL011_CR        (PL011_BASE + 0x30)
#define PL011_IFLS      (PL011_BASE + 0x34)
#define PL011_IMSC      (PL011_BASE + 0x38)
#define PL011_RIS       (PL011_BASE + 0x3C)
#define PL011_MIS       (PL011_BASE + 0x40)
#define PL011_ICR       (PL011_BASE + 0x44)
#define PL011_DMACR     (PL011_BASE + 0x48)
#define PL011_ITCR      (PL011_BASE + 0x80)
#define PL011_ITIP      (PL011_BASE + 0x84)
#define PL011_ITOP      (PL011_BASE + 0x88)
#define PL011_TDR       (PL011_BASE + 0x8C)

#define TXD 14
#define RXD 15

/* Flag register */
#define FR_RXFE (1 << 4)  
#define FR_TXFF (1 << 5)  
#define FR_RXFF (1 << 6)  
#define FR_TXFE (1 << 7)  

/* Line control register */
#define LCRH_FEN  (1 << 4)  
#define LCRH_WLEN_8BIT (3 << 5)  

#define CR_UARTEN (1 << 0)  
#define CR_TXE    (1 << 8)  
#define CR_RXE    (1 << 9)  


void pl011_init();
void pl011_sendc(char c);
char pl011_recv();
void pl011_send_string(char *s);
void handle_uart_irq();