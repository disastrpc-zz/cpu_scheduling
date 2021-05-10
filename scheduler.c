#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "queue.h"
#include "cpu.h"
#include "scheduler.h"

#define TIME_QUANTUM_ 10 // 10ms

/* Loops through the list as is and sends to CPU. Functions in driver.c shuffle the process list
before calling this function, in order to simulate CPU processes being pushed into the queue differently. */
void *fcfs(node *head) {

    int b_time = 0; 
    int t_time = 0;
    int w_time = 0;
    int r_time = 0;
    int size = 0;
    stats *s = malloc(sizeof(stats));
    node *i = head;

    while(i != NULL) {

        // If first node on list
        if(size == 0) {
            i->data.wait = 0;
            i->data.resp = 0;
        } else {
            i->data.wait = b_time;
            i->data.resp = b_time;
        }

        b_time += i->data.burst;
        i->data.turn = b_time;

        w_time += i->data.wait;
        t_time += i->data.turn;

        // Because all processes are assumed to arrived at 0ms responde time will equal wait time
        r_time += i->data.wait;
        
        size++;
        process(i);
        pop(&i, NULL);
    }

    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;
    return s;
}

/* Implements shortest job first scheduling. 
List is walked once per node, */
void *sjf(node *head) {

    int b_time = 0; 
    int t_time = 0;
    int w_time = 0;
    int r_time = 0;
    int size = 0;

    // Used for walking the list
    node *j;
    // Represents min value
    node *m;
    stats *s = malloc(sizeof(stats));
    // Start node
    node *i = head;

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

    i = head;

    while(i != NULL) {

        // If first node on list
        if(size == 0) {
            i->data.wait = 0;
            i->data.resp = 0;
        } else {
            i->data.wait = b_time;
            i->data.resp = b_time;
        }

        b_time += i->data.burst;
        i->data.turn = b_time;

        w_time += i->data.wait;
        t_time += i->data.turn;

        // Because all processes are assumed to arrived at 0ms responde time will equal wait time
        r_time += i->data.wait;
        
        size++;
        process(i);
        pop(&i, NULL);
    }

    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;
    return s;
}

/* Very similar to sjf, instead loop sorts by weight. Returns null pointer to stats struct */
void *ps(node *head) {

    int b_time = 0; 
    int t_time = 0;
    int w_time = 0;
    int r_time = 0;
    int size = 0;

    node *i = head;
    node *j;
    node *m;
    stats *s = malloc(sizeof(stats));

    // Walks lists and sorts according to the process weight
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


    i = head;

    while(i != NULL) {

        // If first node on list
        if(size == 0) {
            i->data.wait = 0;
            i->data.resp = 0;
        } else {
            i->data.wait = b_time;
            i->data.resp = b_time;
        }

        b_time += i->data.burst;
        i->data.turn = b_time;

        w_time += i->data.wait;
        t_time += i->data.turn;

        // Because all processes are assumed to arrived at 0ms responde time will equal wait time
        r_time += i->data.wait;
        
        size++;
        process(i);
        pop(&i, NULL);
    }

    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;
    return s;
}

void *rr(node *head) {

    // Array to store remaining burst time for each process
    // Nodes access this array using their ID as an index
    int b_time[20];

    // Counts iterations to calculate response time
    int c;

    // Size of array and sum of different times
    int size = 0;
    int w_queue;
    int q_time = 0;
    int w_time = 0;
    int t_time = 0;
    int r_time = 0;

    stats *s = malloc(sizeof(stats));
    node *i = head;

    while(i != NULL) {
        b_time[i->data.id] = i->data.burst;
        printf("LOOP 1: %d %d\n", b_time[i->data.id], i->data.id);
        size++;
        i = i->n;
    }

    i = head;
    // Keeps track of processes in waiting queue
    w_queue = size;

    while(1) {

        int d = 1;
        c = 0;
        while(i != NULL) {
            if(b_time[i->data.id] > 0) {

                printf("QTIME: %d\n", q_time);
                
                if(b_time[i->data.id] > TIME_QUANTUM_) {
                    d = 0;

                    q_time += TIME_QUANTUM_;
                    if(w_queue > 0) {
                        i->data.wait = q_time - TIME_QUANTUM_;
                        w_time += i->data.wait;
                    }
                   
                    // Check waiting queue status to only execute on first pass
                    if(w_queue == size) {
                        // Calculate response time for each process
                        i->data.resp = TIME_QUANTUM_ * c;
                        r_time += i->data.resp;
                    }

                    b_time[i->data.id] -= TIME_QUANTUM_;

                } else {

                    q_time += b_time[i->data.id];
                    if(w_queue > 0) {
                        i->data.wait = q_time - b_time[i->data.id];
                        w_time += i->data.wait;
                    }

                    // Check waiting queue status to only execute on first pass
                    if(w_queue == size) {
                        // Calculate response time for each process
                        i->data.resp = b_time[i->data.id] * c;
                        r_time += i->data.resp;
                    }
                    b_time[i->data.id] = 0;
                }

                i->data.turn = q_time;
                t_time += i->data.turn;
                process(i);
            } else {
                // If process is zero remove one from the w_queue
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

    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;

    return s;
}

void
void *prr(node *head) {

    int b_time[20];
    int t_time = 0;
    int w_time = 0;
    int r_time = 0;
    int w_queue = 0;
    int q_time = 0;
    int c = 0;
    int size = 0;
    int c_weight = 0;

    node *i = head;
    node *j;
    node *m;
    stats *s = malloc(sizeof(stats));

    // Walks lists and sorts according to the weight
    while(i != NULL) {
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

    i = head;
    node *tmp;
    task *t = malloc(sizeof(task));

    while(i != NULL) {

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
    } else if(strcmp(a, "prr") == 0) {
        s = prr(head);
    }

    return s;
}
