#include "os_core/process.h"
#include "printf.h"
#include "interrupt.h"

static pcb init_task = INIT_TASK;
pcb *current = &init_task;
pcb *task_table[T_LIST] = {&init_task, };
int nr_tasks = 1;
queue queues[MAX_QUEUE];

static uint64_t system_ticks = 0;
static uint64_t last_boost = 0;

static void _schedule(void);

void preempt_enable() { 
    current->preempt_count--; 
    printf("[PREEMPT] Enabled: PID %d, count=%ld\n", 
           current->pid, current->preempt_count);
}

void preempt_disable() { 
    current->preempt_count++; 
    printf("[PREEMPT] Disabled: PID %d, count=%ld\n", 
           current->pid, current->preempt_count);
}

void schedule_tail() {
    printf("[SCHEDULE_TAIL] PID %d starting execution\n", current->pid);
    preempt_enable();
}

void schedule(void)
{
    printf("[SCHEDULE] Voluntary yield: PID %d\n", current->pid);
    current->counter = 0;
    _schedule();
}

void sched_init() {
    printf("\n========================================\n");
    printf("Initializing MLFQ Scheduler\n");
    printf("========================================\n");
    
    for (int i = 0; i < MAX_QUEUE; i++) {
        queues[i].count = 0;
        for (int j = 0; j < T_LIST; j++) {
            queues[i].tasks[j] = 0;
        }
        printf("  Q%d initialized: %d tasks\n", i, queues[i].count);
    }
    
    system_ticks = 0;
    last_boost = 0;
    
    printf("\nTime slices:\n");
    printf("  Q0: %d ticks (highest priority)\n", TIMESLICE_Q0);
    printf("  Q1: %d ticks (medium priority)\n", TIMESLICE_Q1);
    printf("  Q2: %d ticks (lowest priority)\n", TIMESLICE_Q2);
    printf("  Boost period: %d ticks\n", BOOST_PERIOD);
    printf("========================================\n\n");
}

int get_timeslice(int queue_level)
{
    if (queue_level == 0) return TIMESLICE_Q0;
    if (queue_level == 1) return TIMESLICE_Q1;
    return TIMESLICE_Q2;
}

void check_and_demote(pcb* task) {
    if (task == &init_task) {
        return;
    }
    
    if (task->queue_level < MAX_QUEUE - 1) {
        int old_level = task->queue_level;
        task->queue_level++;
        printf("[DEMOTE] PID %d: Q%d -> Q%d (used full time slice)\n", 
               task->pid, old_level, task->queue_level);
    } else {
        printf("[DEMOTE] PID %d: Already at lowest queue Q%d\n", 
               task->pid, task->queue_level);
    }
}

void priority_boost(void) {
    printf("\n****************************************\n");
    printf("[BOOST] Priority boost at tick %lu!\n", system_ticks);
    printf("****************************************\n");
    
    int total_moved = 0;
    
    for (int q = 1; q < MAX_QUEUE; q++) {
        int moved = 0;
        while (queues[q].count > 0) {
            pcb* task = dequeue(q);
            if (task) {
                printf("[BOOST] Moving PID %d from Q%d -> Q0\n", 
                       task->pid, q);
                task->queue_level = 0;
                enqueue(0, task);
                moved++;
                total_moved++;
            }
        }
        if (moved > 0) {
            printf("[BOOST] Moved %d tasks from Q%d\n", moved, q);
        }
    }
    
    if (current != &init_task) {
        printf("[BOOST] Current task PID %d -> Q0\n", current->pid);
        current->queue_level = 0;
    }
    
    printf("[BOOST] Total %d tasks boosted\n", total_moved);
    printf("****************************************\n\n");
    
    last_boost = system_ticks;
}

void _schedule() {
    printf("\n[SCHEDULE] === Scheduling Decision ===\n");
    printf("[SCHEDULE] Current: PID %d, Q%d, counter=%ld, ticks=%lu\n",
           current->pid, current->queue_level, current->counter, current->ticks);
    
    preempt_disable();
    
    if (current != &init_task && current->state == TASK_RUNNING) {
        printf("[SCHEDULE] Saving PID %d to Q%d\n", 
               current->pid, current->queue_level);
        
        if (current->counter == 0) {
            printf("[SCHEDULE] Time slice expired for PID %d\n", current->pid);
            check_and_demote(current);
        }
        
        enqueue(current->queue_level, current);
    } else if (current == &init_task) {
        printf("[SCHEDULE] Init task (idle), not saving\n");
    }
    
    printf("[SCHEDULE] Queue status:\n");
    for (int i = 0; i < MAX_QUEUE; i++) {
        printf("  Q%d: %d tasks [", i, queues[i].count);
        for (int j = 0; j < queues[i].count; j++) {
            printf("PID%d ", queues[i].tasks[j]->pid);
        }
        printf("]\n");
    }
    
    pcb* next = pick_next_task();
    if (!next) {
        printf("[SCHEDULE] No tasks ready, running init_task\n");
        next = &init_task;
    }
    
    printf("[SCHEDULE] Selected: PID %d from Q%d\n", 
           next->pid, next->queue_level);
    
    next->counter = get_timeslice(next->queue_level);
    next->state = TASK_RUNNING;
    
    printf("[SCHEDULE] Giving PID %d time slice of %ld ticks\n",
           next->pid, next->counter);
    printf("[SCHEDULE] === End Scheduling ===\n\n");
    
    switch_to(next);
    preempt_enable();
}

void switch_to(pcb* next) 
{
    if (current == next) {
        printf("[SWITCH] Already running PID %d, no switch needed\n", 
               next->pid);
        return;
    }
    
    printf("[SWITCH] Context switch: PID %d -> PID %d\n", 
           current->pid, next->pid);
    
    pcb* prev = current;
    current = next;
    cpu_switch_to(prev, next);
}

pcb* pick_next_task() {
    printf("[PICK] Searching for next task...\n");
    
    for (int i = 0; i < MAX_QUEUE; i++) {
        if (queues[i].count > 0) {
            pcb* task = dequeue(i);
            printf("[PICK] Found PID %d in Q%d (MLFQ Rule 1)\n", 
                   task->pid, i);
            return task;
        }
    }
    
    printf("[PICK] No tasks in any queue\n");
    return &init_task;
}

void timer_tick() {
    system_ticks++;
    current->ticks++;
    current->counter--;
    
    if (system_ticks % 10 == 0) {
        printf("[TICK %lu] PID %d: Q%d, counter=%ld, total_ticks=%lu\n",
               system_ticks, current->pid, current->queue_level, 
               current->counter, current->ticks);
    }
    
    if (system_ticks - last_boost >= BOOST_PERIOD) {
        printf("[TICK] Boost period reached!\n");
        enable_irq();
        priority_boost();
        disable_irq();
    }
    
    if (current->counter > 0 || current->preempt_count > 0) {
        if (current->preempt_count > 0 && system_ticks % 50 == 0) {
            printf("[TICK] Preemption disabled (count=%ld)\n", 
                   current->preempt_count);
        }
        return;
    }
    
    printf("[TICK] Time slice expired for PID %d\n", current->pid);
    current->counter = 0;
    
    enable_irq();
    _schedule();
    disable_irq();
}

void enqueue(int priority, pcb* p) {
    if (priority >= MAX_QUEUE || priority < 0) {
        printf("[ENQUEUE] ERROR: Invalid queue %d\n", priority);
        return;
    }
    
    if (!p) {
        printf("[ENQUEUE] ERROR: NULL task pointer\n");
        return;
    }
    
    if (queues[priority].count >= T_LIST) {
        printf("[ENQUEUE] ERROR: Queue %d is full!\n", priority);
        return;
    }
    
    queues[priority].tasks[queues[priority].count] = p;
    p->state = TASK_RUNNING;
    queues[priority].count++;
    
    printf("[ENQUEUE] PID %d added to Q%d (count now %d)\n",
           p->pid, priority, queues[priority].count);
}

pcb* dequeue(int priority) {
    if (priority >= MAX_QUEUE || priority < 0) {
        printf("[DEQUEUE] ERROR: Invalid queue %d\n", priority);
        return 0;
    }
    
    if (queues[priority].count == 0) {
        printf("[DEQUEUE] Queue %d is empty\n", priority);
        return 0;
    }
    
    pcb* task = queues[priority].tasks[0];
    
    for (int i = 0; i < queues[priority].count - 1; i++) {
        queues[priority].tasks[i] = queues[priority].tasks[i + 1];
    }
    
    queues[priority].tasks[queues[priority].count - 1] = 0;
    queues[priority].count--;
    
    printf("[DEQUEUE] PID %d removed from Q%d (count now %d)\n",
           task->pid, priority, queues[priority].count);
    
    return task;
}