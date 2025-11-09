#pragma once
#include "base.h"

#define PAGE_SHIFT 12 // 2^12 = 4kB each page is 4kb
#define TABLE_SHIFT 9 // 2^9 = 512 entries per table
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT) // 21 2^21 = 2MB section
#define PAGE_SIZE (1 << PAGE_SHIFT) // 4kB
#define SECTION_SIZE (1 << SECTION_SHIFT) // 2MB section
#define LOW_MEMORY (2 * SECTION_SIZE) // 4Mb (Reserved) Kernel + Stack
#define HIGH_MEMORY RPI_PERIPHERAL_BASE

/* Memory area we can use for paging
 * High memory is reserved for bcm2711 defines
 * Low memory is given for stack
 * We can use the remaining memory for page tables */
#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES (PAGING_MEMORY / PAGE_SIZE)

#ifndef __ASSEMBLER__
void memzero(unsigned long src, unsigned int n);
int get_el(void);
uint64_t get_free_pages(void);
void free_page(uint64_t page_address);
#endif