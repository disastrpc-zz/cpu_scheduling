// Queue implements a linked list which represents the process queue. 
#include "driver.h"
#ifndef QUEUE_
#define QUEUE_

// Holds task struct which holds process data, 
// and pointer to next item in list
typedef struct node {
    task data;
    struct node *n;
} node;

// Print items in list
void print(node *p);

// Push item to the start of queue, also used to create the first node of the list 
void push(node **p_head, task *p_task);

// Pops item from start of queue
void pop(node **p_head, task *p_task); 

// Adds item to the end of queue
void skip(node *p_head, task *p_task);

// Separate item at the end of queue
void separate(node *p_head);

// Pulls node from list by index and stores in task stuct
void pull(node **p_head, task *p_task, unsigned int index);

void swap(node *n1, node *n2);
#endif
