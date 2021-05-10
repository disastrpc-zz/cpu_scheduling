/*
These functions implement a linked list that hold the data for each processor task
*/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "driver.h"

// Takes pointer to head of list and walks it
void print(node *head) {

    node *i = head;

    printf("[*] Printing nodes...\n");
    while(i != NULL) {
        printf("[*] PID: %d WEIGHT: %d CPU BURST: %d\n", i->data.id, i->data.weight, i->data.burst);
        i = i->n;
    }
}

void push(node **p_head, task *item) {

    // Allocates memory for new list node
    node *n_item = (node*) malloc(sizeof(node));

    if(item == NULL) {
        perror("[-] ERROR: Item not provided");
        exit(-1);
    }

    // Assign data from task struct to node data
    n_item->data.id = item->id;
    n_item->data.weight = item->weight;
    n_item->data.burst = item->burst;
    n_item->data.resp = item->resp;
    n_item->data.turn = item->turn;
    n_item->data.wait = item->wait;

    // Set node's next pointer to the previous head of the list
    n_item->n = (*p_head);
    n_item->p = NULL;

    if((*p_head) != NULL) {
        (*p_head)->p = n_item;
    }

    // Changes the head to the new item
    *p_head = n_item;

}

// Takes head pointer and pointer to already allocated 
// task struct, which will be modified to popped values
void pop(node **p_head, task *p_task) {

    node * n_node = NULL;

    n_node = (*p_head)->n;

    if(p_task != NULL) {
        p_task->id = (*p_head)->data.id;
        p_task->weight = (*p_head)->data.weight;
        p_task->burst = (*p_head)->data.burst;
        p_task->resp = (*p_head)->data.resp;
        p_task->turn = (*p_head)->data.turn;
        p_task->wait = (*p_head)->data.wait;
    }

    free(*p_head);
    *p_head = n_node;
}

void swap(node *n1, node *n2) {
    
    node tmp;
    tmp.data = n1->data;
    n1->data = n2->data;
    n2->data = tmp.data;

}


void del(node **head, int index) {

    // Check if list is empty
    if(*head == NULL) {
        perror("[-] ERROR: Unable to delete node");
        exit(-1);
    }

    node *i = *head;

    // Special case for removing head of list
    if(index == 0) {
        *head = i->n;
        free(i);
        return;
    }

    // Find node before the node to be deleted
    for(int j = 0; i != NULL && j < index - 1; j++) {
        i = i->n;
    }

    // Check if current position is valid
    if(i == NULL || i->n == NULL) {
        perror("[-] ERROR: Unable to delete node");
        exit(-1);
    }

    // Store pointer after node to be deleted
    node *n = i->n->n;

    // Remove node and free memory
    free(i->n);
    i->n = n;

}