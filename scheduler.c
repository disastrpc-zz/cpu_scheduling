#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "queue.h"
#include "cpu.h"
#include "scheduler.h"

#define TIME_QUANTUM_ 10 // 10ms

/* Accepts pointer to task and stats structs as well as the size of the queue
   Modifies the stats struct to the average turnaround, wait and reponse values */ 
void gen_stats(task *t, stats *s, int size) {

    // Turnaround = all cpu bursts + 1ms for each process to arrive / the number of processes in queue
    s->avg_turn = (t->turn + size ) / size;
    // Average wait = wait time + 1ms per process / the number of processes 
    s->avg_wait = (t->wait + size ) / size;
    // Average response = all cpu bursts - 1ms per process / number of processes
    s->avg_resp = (t->resp - size) / size;
    
}

/* Loops through the list as is and sends to CPU. Functions in driver.c shuffle the process list
before calling this function, in order to simulate CPU processes getting to the queue at different times. */
void *fcfs(node *head) {

    int c, k = 0;
    task *t = malloc(sizeof(task));
    stats *s = malloc(sizeof(stats));
    node *i = head;

    while(i != NULL) {

        if(i->n == NULL) {
            i->data.wait = (c - i->data.burst) + k;
        } else {
            i->data.wait = c + k;
        }

        i->data.resp += c - k;
        c += i->data.burst;
    
        i->data.turn = (i->data.burst + i->data.wait) + k;
        k++;
        process(i);
        pop(&i, t);
    }

    gen_stats(t, s, k);
    return s;
}

/* Implements shortest job first scheduling. 
List is walked once per node, */
void *sjf(node *head) {

    // Start node
    node *i = head;
    // Used for walking the list
    node *j;
    // Represents min value
    node *m;
    task *t = malloc(sizeof(task));
    stats *s = malloc(sizeof(stats));

    // Walks lists and sorts according to the CPU burst
    while(i->n) {
        m = i;
        j = i->n;

        // Inner loop compares minimum element with element from next node
        while(j) {
            if(m->data.burst > j->data.burst) {
                m = j;
            }
            // Advance transition node
            j = j->n;
        }
        
        // Swap current node with min node
        swap(i, m);
        i = i->n;
    }


    int c, k;
    c = 0;
    k = 0;
    i = head;

    while(i != NULL) {

        // If we're at the last node of the list substract the burst from the wait time
        if(i->n == NULL) {
            i->data.wait = (c - i->data.burst) + k;
        } else {
            i->data.wait = c + k;
        }
        
        // Process response time is the sum of all bursts minus 1ms per process
        i->data.resp += c - k;

        c += i->data.burst;
    
        // Turnaround time is b[p] + w[p]
        i->data.turn = (i->data.burst + i->data.wait) + k;

        k++;

        // Send the process to the CPU and pop it from the queue
        process(i);
        pop(&i, t);
    }

    // Get process statistics
    gen_stats(t, s, k);

    return s;
}

/* Very similar to sjf, instead loop sorts by weight. Returns null pointer to stats struct */
void *ps(node *head) {

    node *i = head;
    node *j;
    node *m;
    task *t = malloc(sizeof(task));
    stats *s = malloc(sizeof(stats));

    // Walks lists and sorts according to the CPU burst
    while(i->n) {
        m = i;
        j = i->n;

        // Inner loop compares minimum element with element from next node
        while(j) {
            // Sort by process weight
            if(m->data.weight > j->data.weight) {
                m = j;
            }

            j = j->n;
        }
        
        swap(i, m);
        i = i->n;
    }


    int c, k;
    c = 0;
    k = 0;
    i = head;

    // Calculate statistics for each node
    while(i != NULL) {

        if(i->n == NULL) {
            i->data.wait = (c - i->data.burst) + k;
        } else {
            i->data.wait = c + k;
        }

        i->data.resp += c - k;
        c += i->data.burst;
    
        i->data.turn = (i->data.burst + i->data.wait) + k;
        k++;
        process(i);
        pop(&i, t);
    }

    // Generate statistics
    gen_stats(t, s, k);

    return s;
}

void *rr(node *head) {

    int r_time[20];
    int size = 0;
    int q_time = 0;
    node *i = head;

    while(i != NULL) {
        r_time[i->data.id] = i->data.burst;
        printf("LOOP 1: %d %d\n", r_time[i->data.id], i->data.id);
        size++;
        i = i->n;
    }

    i = head;
    int c;
    int c_burst;
    int w_queue = size;

    while(1) {

        int d = 1;
        c = 1;
        while(i != NULL) {
            if(r_time[i->data.id] > 0) {

                d = 0;
                
                if(r_time[i->data.id] > TIME_QUANTUM_) {
                    d = 0;
                    q_time += TIME_QUANTUM_;
                    r_time[i->data.id] -= TIME_QUANTUM_;
                } else {
                    q_time += r_time[i->data.id];
                    r_time[i->data.id] = 0;
                }
                
                if(w_queue > 0) {
                    c_burst += r_time[i->data.id];
                    i->data.wait = q_time;
                }

                if(w_queue == size) {
                    i->data.resp = TIME_QUANTUM_ * c;
                }

                i->data.turn = c_burst;

                process(i);
            } else {
                w_queue -= 1;
            }

            if(i->n == NULL) {
                break;
            } else {
                i = i->n;
            }

            c++;
        }

        if(d == 1) {
            break;
        } else {
            i = head;
        }
    }
}

void test(node *head) {
    node *n1 = head;
    node *n2;

    printf("N1:\n");
    print(n1);
    n2 = copy(n1);
    printf("N2\n");
    print(n2);
}

void *schedule(node *head, char a[]) {

    stats *s;
    if(strcmp(a, "fcfs") == 0) {
        s = fcfs(head);
    } else if(strcmp(a, "sjf") == 0) {
        s = sjf(head);
    } else if(strcmp(a, "ps") == 0) {
        s = ps(head);
    } else if(strcmp(a, "rr") == 0){
        s = rr(head);
    } else if(strcmp(a, "test") == 0) {
        test(head);
    }

    return s;
}
