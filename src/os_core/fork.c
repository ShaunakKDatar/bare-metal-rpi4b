#include "os_core/fork.h"

int fork(uint64_t fn_ptr, uint64_t arg){
    preempt_disable();
    pcb *p;
    p = (pcb*)get_free_pages();
    if (!p){
        return -1;
    }
    p->state = TASK_RUNNING;
    p->counter = 0;
    p->queue_level = 0;
    p->preempt_count = 1;
    p->regs.x19 = fn_ptr;
    p->regs.x20 = arg;
    p->regs.pc = (unsigned long)ret_from_fork;
    p->regs.sp = (unsigned long)p + THREAD_SIZE;
    p->pid = nr_tasks;
    int pid = nr_tasks++;
    task_table[pid] = p;

    enqueue(0, p);
    preempt_enable();
    return pid;
}