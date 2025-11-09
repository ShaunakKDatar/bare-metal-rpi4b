#include "mm.h"

static unsigned short mem_map[PAGING_PAGES] = {
    0,
};

uint64_t get_free_pages(){
    for(int i = 0; i < PAGING_PAGES; i++){
        if(mem_map[i] == 0){
            mem_map[i] = 1;
            return LOW_MEMORY + i*PAGE_SIZE;
        }
    }
    return 0;
}

void free_page(uint64_t page_address){
    mem_map[(page_address - LOW_MEMORY) / PAGE_SIZE] = 0;
}