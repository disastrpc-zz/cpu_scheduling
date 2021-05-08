#include "queue.h"
#include "driver.h"

#ifndef SCHEDULER_
#define SCHEDULER_

typedef struct stats {
    int avg_turn;
    int avg_wait;
    int avg_resp;
} stats;

void *schedule(node *head, char a[]);

#endif