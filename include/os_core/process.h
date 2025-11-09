#pragma once
#include "base.h"
#include "interrupt.h"
#include "fork.h"

#define T_LIST  64
#define MAX_QUEUE 3
#define THREAD_SIZE 4096

#define TIMESLICE_Q0 10    // Highest priority: 10 ticks
#define TIMESLICE_Q1 20    // Medium priority: 20 ticks
#define TIMESLICE_Q2 40    // Lowest priority: 40 ticks
#define BOOST_PERIOD 1000  // Priority boost every 1000 ticks


#define TASK_RUNNING 0
#define TASK_WAITING 1

#define INIT_TASK \
/* cpu_context */ { {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state */        TASK_RUNNING, \
/* pid */          0, \
/* counter */      TIMESLICE_Q0, \
/* preempt_count */0, \
/* queue_level */  0, \
/* ticks */        0 \
}

typedef struct cpu_context {
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t fp;
    uint64_t sp;
    uint64_t pc;
}cpu_context;

typedef struct pcb {
    cpu_context regs; /* CPU REGS */
    long state; /* TASK_RUNNING or WAITING */
    int pid; /* Process ID */
    long counter; /* Time left in current slice */
    long preempt_count; /* To Disable preemption 1->disable */
    int queue_level; /* Priority decider in MLFQ */
    uint64_t ticks; /* Total runtime */
}pcb;


typedef struct queue {
    pcb* tasks[T_LIST];
    int count;
}queue;

extern pcb *current;
extern pcb *task_table[T_LIST];
extern int nr_tasks;
extern queue queues[MAX_QUEUE];

void preempt_enable();
void preempt_disable();
void timer_tick();
void schedule_tail();
void schedule(void);
void enqueue(int priority, pcb* p);
pcb* dequeue(int priority);
void sched_init();
int get_timeslice(int queue_level);
void check_and_demote(pcb* task);
void priority_boost(void);
void switch_to(pcb* next);
pcb* pick_next_task();
void timer_tick();
void cpu_switch_to(pcb* prev, pcb* next);