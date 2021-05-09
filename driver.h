#ifndef DRIVER_
#define DRIVER_

/* Defines task to be added to linked list node and its properties */
typedef struct task {
    unsigned int id;
    unsigned int weight;
    unsigned int burst;
    unsigned int resp;
    unsigned int turn;
    unsigned int wait;
} task;


#endif