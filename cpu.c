#include <stdio.h>
#include "queue.h"

void process(node *t) {
    printf("[CPU EXECUTING]\n[ID: %d]\n[WEIGHT: %d]\n[BURST TIME: %d]\n\n", t->data.id, t->data.weight, t->data.burst);
}