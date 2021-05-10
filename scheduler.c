/* Includes all scheduling algorithms and a schedule() function which calls them 
Results are passed to the CPU's process() function to execute them, results are displayed by driver.c 
All functions take pointer to the head of a linked list containing the queue */
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

    // Values used to calculate average times
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

        // Keeps track of average burst, waiting, turnaround and response time
        b_time += i->data.burst;
        i->data.turn = b_time;
        w_time += i->data.wait;
        t_time += i->data.turn;
        // Because all processes are assumed to arrived at 0ms response time will equal wait time
        r_time += i->data.wait;
        
        size++;
        process(i);
        pop(&i, NULL);
    }

    // Avg = time / size of list
    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;
    return s;
}

/* Implements shortest job first scheduling. 
List is walked once per node, inner loop is used to compare each element recursively */
void *sjf(node *head) {
    // Values used to calculate average times
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

    // Loop through list in order to generate statistics
    while(i != NULL) {

        // If first node on list
        if(size == 0) {
            i->data.wait = 0;
            i->data.resp = 0;
        } else {
            i->data.wait = b_time;
            i->data.resp = b_time;
        }

        // Keeps track of total burst, waiting, turnaround and response times for entire queue
        b_time += i->data.burst;
        i->data.turn = b_time;
        w_time += i->data.wait;
        t_time += i->data.turn;
        r_time += i->data.wait;
        
        size++;
        process(i);
        pop(&i, NULL);
    }

    // Avg = time / size of list
    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;
    return s;
}

/* Very similar to sjf, instead loop sorts by weight. Returns void pointer to stats struct */
void *ps(node *head) {

    // Values used to calculate average times
    int b_time = 0; 
    int t_time = 0;
    int w_time = 0;
    int r_time = 0;
    int size = 0;

    node *i = head;
    node *j;
    node *m;
    stats *s = malloc(sizeof(stats));

    // List is sorted by weight
    while(i->n) {
        m = i;
        j = i->n;

        while(j) {
            if(m->data.weight > j->data.weight) {
                m = j;
            }

            j = j->n;
        }
        
        swap(i, m);
        i = i->n;
    }


    i = head;

    // Loop through list in order to generate statistics
    while(i != NULL) {

        // If first node on list
        if(size == 0) {
            i->data.wait = 0;
            i->data.resp = 0;
        } else {
            i->data.wait = b_time;
            i->data.resp = b_time;
        }

        // Keeps track of total burst, waiting, turnaround and response times for entire queue
        b_time += i->data.burst;
        i->data.turn = b_time;
        w_time += i->data.wait;
        t_time += i->data.turn;
        r_time += i->data.wait;
        
        size++;
        process(i);
        pop(&i, NULL);
    }

    // Avg = time / size of list
    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;
    return s;
}

/* Round robin scheduling loops through the queue multiple times executing each process for one time quantum, in this case
hardcoded to 10ms, until all CPU bursts are finished. This provides much greater response time but higher waiting and turnaround times. */
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
        size++;
        i = i->n;
    }

    i = head;
    // Keeps track of processes in waiting queue
    w_queue = size;

    while(1) {

        // Bool integer to mark when queue is done in order to break loop
        int d = 1;
        c = 0;
        while(i != NULL) {
            if(b_time[i->data.id] > 0) {
                
                // If remaining time is larger than TQ process is not done
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

                    // Only add remaining time to total waiting
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

/* Executes tasks by priority, falling back to RR if two or more share the same weight. It has a slighly larger response time by waiting
and turnaround times are improved from regular RR */
void *prr(node *head) {

    int c = 0;
    int size = 0;
    int c_weight = 0;
    int b_time = 0;
    int w_time = 0;
    int r_time = 0;
    int t_time = 0;

    node *i = head;
    node *j;
    node *m;
    stats *s = malloc(sizeof(stats));

    // Walks lists and sorts according to the weight for further processing
    while(i != NULL) {

        // Stores current list node
        m = i;

        // Node stores next spot in list
        j = i->n;

        while(j) {
            if(m->data.weight > j->data.weight) {
                m = j;
            }

            j = j->n;
        }
        
        swap(i, m);
        i = i->n;
        c++;
    }

    i = head;
    node *tmp;
    task *t = malloc(sizeof(task));
    
    // Walks ordered list
    while(i != NULL) {
        if(i->n != NULL) {

            // If next node's weight is equal to this node, start loop
            if(i->data.weight == i->n->data.weight) {

                // Copy pointer and set variable to track last used weight
                // Loops until the weight of next node isn't equal anymore
                node *t_ptr = i;
                int t_weight = t_ptr->data.weight;
                while(t_ptr->data.weight == t_weight) {
                    t->id = t_ptr->data.id;
                    t->weight = t_ptr->data.weight;
                    t->burst = t_ptr->data.burst;
                    // Add to sublist
                    push(&tmp, t);
                    process(tmp);
                    t_ptr = t_ptr->n;
                    i = t_ptr;
                    i = i->n;
                }

                // Send sublist to RR function for processing
                s = rr(tmp);
                r_time = s->avg_resp;
                t_time = s->avg_turn;
                w_time = s->avg_wait;
                continue;

            }
        } 


        if(size == 0) {
            i->data.wait = 0;
            i->data.resp = 0;
        } else {
            i->data.wait = b_time;
            i->data.resp = b_time;
        }


        // Keeps track of average times
        b_time += i->data.burst;
        i->data.turn = b_time;
        w_time += i->data.wait;
        t_time += i->data.turn;
        r_time += i->data.wait;

        process(i);
        i = i->n;
        size++;
    }
    
    // Avg = time / size of list
    s->avg_wait = w_time / size;
    s->avg_turn = t_time / size;
    s->avg_resp = r_time / size;

    return s;
}

// function to call different algorithms
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
    } else {
        printf("[-] ERROR: Invalid option %s, execute without arguments for help.\n", a);
        exit(-1);
    }

    return s;
}
