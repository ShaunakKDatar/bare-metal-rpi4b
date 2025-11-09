#include "base.h"
#include "mm.h"
#include "process.h"
#include "interrupt.h"

int fork(uint64_t fn_ptr, uint64_t arg);
void ret_from_fork(void);