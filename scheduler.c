#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "cpu.h"
#include "scheduler.h"

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
    
        i->data.turn = c;
        k++;
        process(i);
        pop(&i, t);
    }

    gen_stats(t, s, k);
    return s;
}

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

            j = j->n;
        }
        
        swap(i, m);
        i = i->n;
    }


    int c, k;
    c = 0;
    k = 0;
    i = head;

    while(i != NULL) {

        if(i->n == NULL) {
            i->data.wait = (c - i->data.burst) + k;
        } else {
            i->data.wait = c + k;
        }

        i->data.resp += c - k;
        c += i->data.burst;
    
        i->data.turn = c;
        k++;
        process(i);
        pop(&i, t);
    }

    gen_stats(t, s, k);

    return s;
}

void *ps(node *head) {

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

    while(i != NULL) {

        if(i->n == NULL) {
            i->data.wait = (c - i->data.burst) + k;
        } else {
            i->data.wait = c + k;
        }

        i->data.resp += c - k;
        c += i->data.burst;
    
        i->data.turn = c;
        k++;
        process(i);
        pop(&i, t);
    }

    gen_stats(t, s, k);

    return s;
}

void *schedule(node *head, char a[]) {

    stats *s;
    if(strcmp(a, "fcfs") == 0) {
        s = fcfs(head);
    } else if(strcmp(a, "sjf") == 0) {
        s = sjf(head);
    } else if(strcmp(a, "ps") == 0) {
        s = ps(head);
    }

    return s;
}
