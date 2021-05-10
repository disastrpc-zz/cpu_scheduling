/* Queue implements a linked list which represents the process queue. */

#include "driver.h"
#ifndef QUEUE_
#define QUEUE_

// Holds task struct which holds process data, 
// and pointer to next item in list
typedef struct node {
    task data;
    struct node *n;
    struct node *p;
} node;

// Print items in list
void print(node *p);

// Push item to the start of queue, also used to create the first node of the list 
void push(node **p_head, task *p_task);

// Pops item from start of queue
void pop(node **p_head, task *p_task); 

// Copy list 1 into list 2
void *copy(node *head);

// Swaps data from n1 and n2
void swap(node *n1, node *n2);

void del(node **head, int index);
#endif
