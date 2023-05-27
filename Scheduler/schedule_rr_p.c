#include "schedule_rr.h"
#include "list.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "CPU.h"

#define MAX_STACKS 10

struct node *stack[MAX_STACKS];
int quantum = 30;

// add a task to the list
void add(char *name, int priority, int burst){
    struct task *task = malloc(sizeof(struct task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    
    push(&stack[priority - 1], task);
}

// invoke the scheduler
void schedule(){
    for (int i = 0; i < MAX_STACKS; i++) {
        if (stack[i] == 0) {
            stack[i] = malloc(sizeof (struct node));
            continue;
        }
        while (stack[i]->next) {
            struct node *next = stack[i]->next;
            if (stack[i]->task->burst > quantum) {
                run(stack[i]->task, quantum);
                stack[i]->task->burst -= quantum;
                stack[i]->task->priority++;
                push(&stack[i + 1], stack[i]->task);
                free(stack[i]);
            } else {
                run(stack[i]->task, stack[i]->task->burst);
                free(stack[i]);
            }
            stack[i] = next;
        }
        if (stack[i]->task->burst > quantum) {
            if (i == MAX_STACKS - 1) {
                while (stack[i]->task->burst > quantum) {
                    run(stack[i]->task, quantum);
                    stack[i]->task->burst -= quantum;
                }
                run(stack[i]->task, stack[i]->task->burst);
            } else {
                run(stack[i]->task, quantum);
                stack[i]->task->burst -= quantum;
                stack[i]->task->priority++;
                push(&stack[i + 1], stack[i]->task);
            }
        } else {
            run(stack[i]->task, stack[i]->task->burst);
        }
        free(stack[i]);
    }
}
