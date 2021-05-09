#include "queue.h"
#include "driver.h"

#ifndef SCHEDULER_
#define SCHEDULER_

// Contains avg stats of an entire process queue
typedef struct stats {
    int avg_turn;
    int avg_wait;
    int avg_resp;
} stats;

/* Main schedule function. Takes pointer to head of list. 
'a' parameter determines the type of scheduling to be done.
fcfs
sjf
ps
rr
psrr 
*/
void *schedule(node *head, char a[]);

#endif