#include <stdio.h>
#include "queue.h"

void process(node *t) {
    printf("[CPU EXECUTING]\n[ID: %d]\n[WEIGHT: %d]\n[BURST TIME: %d]\n", t->data.id, t->data.weight, t->data.burst);
    printf("[WAITING TIME: %dms]\n", t->data.wait);
    printf("[TURNAROUND TIME: %dms]\n", t->data.turn);
    printf("[RESPONSE TIME: %dms]\n\n", t->data.resp);
}