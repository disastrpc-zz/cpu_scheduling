/*
These functions implement a linked list that hold the data for each processor task
*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "driver.h"

// Takes pointer to head of list and walks it
void print(node *p_head) {

    node *i = p_head;

    printf("[*] Printing nodes...\n");
    while(i != NULL) {
        printf("[*] PID: %d WEIGHT: %d CPU BURST: %d\n", i->data.id, i->data.weight, i->data.burst);
        i = i->n;
    }
}

void push(node **p_head, task *item) {

    // Allocates memory for new list node
    node *n_item = (node*) malloc(sizeof(node));

    // Assign data from task struct to node data
    n_item->data.id = item->id;
    n_item->data.weight = item->weight;
    n_item->data.burst = item->burst;

    // Set node's next pointer to the previous head of the list
    n_item->n = *p_head;

    // Changes the head to the new item
    *p_head = n_item;

}

// Takes head pointer and pointer to already allocated 
// task struct, which will be modified to popped values
void pop(node **p_head) {

    node * n_node = NULL;

    n_node = (*p_head)->n;
    // p_task->id = (*p_head)->data.id;
    // p_task->weight = (*p_head)->data.weight;
    // p_task->burst = (*p_head)->data.burst;

    free(*p_head);
    *p_head = n_node;
}

// Places task at the end of queue, skipping it
void skip(node *p_head, task *p_task) {

    node *i = p_head;

    // Iterate through list until the end is reached
    while(i->n != NULL) {
        i = i->n;
    }

    node *n_node = (node*) malloc(sizeof(node));

    n_node->data.id = p_task->id;
    n_node->data.weight = p_task->weight;
    n_node->data.burst = p_task->burst;
    n_node->n->n = NULL;
    
}

// Separates item from the end of queue in typical FIFO fashion
// Takes pointer to the list head and a task struct pointer to modify
void separate(node *p_head) {
    
    // Check if list contains only one item
    if(p_head->n == NULL) {

        // Assign values and free current head's memory
        // p_task->id = p_head->data.id;
        // p_task->weight = p_head->data.weight;
        // p_task->burst = p_head->data.burst;
        free(p_head);

        return;
    }

    // Otherwise we must look forward two indexes in the list
    // in order to change the last item and the one before
    // This moves our pointer to the item before the last index
    node *i = p_head;
    while(i->n->n != NULL) {
        i = i->n;
    }

    // Now we can use the pointer to access the next spot 
    // on the list to assign data, free memory, and set the
    // next variable to NULL on the current node
    // p_task->id = i->n->data.id;
    // p_task->weight = i->n->data.weight;
    // p_task->burst = i->n->data.burst;
    free(i->n);
    i->n = NULL;
}

void pull(node **p_head, task *p_task, unsigned int index) {

    node *i = *p_head;
    node *t_node = NULL;

    // To avoid iterating over the list simply invoke pop() if index if 0
    if(index == 0) {
        pop(p_head);
    }

    // Iterate pointer to spot before specified index 
    for(int n = 0; n < index - 1; n++) {

        // Check if next is NULL, if it is something went wrong
        if(i->n == NULL) {
            perror("[-] ERROR: Invalid index");
            return;
        }

        // Grab value from next to advance pointer
        i = i->n;
    }

    // Store the last index in a temprary node
    t_node = i->n;

    // Assign data to provided struct from temporary node
    p_task->id = t_node->data.id;
    p_task->weight = t_node->data.weight;
    p_task->burst = t_node->data.burst;

    // Overwrite with temporary node
    i->n = t_node->n;

    free(t_node);
}