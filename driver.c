#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "driver.h"
#include "queue.h"
#include "scheduler.h"


// Returns pointer to task struct, caller is responsible for freeing allocated memory
task *build_task(char t_str[10]) {
    // Initialize string and int temp variables to hold task properties
    char sid[10], sw[10], sb[10];
    int id, w, b;

    // Strip the delimeter and cast to int
    strcpy(sid, strtok(t_str, ":"));
    strcpy(sw, strtok(NULL, ":"));
    strcpy(sb, strtok(NULL, ":"));
    id = atoi(sid);
    w = atoi(sw);
    b = atoi(sb);
    
    // Allocate task resources and assign properties
    task *t = malloc(sizeof(task));
    t->id = id;
    t->weight = w;
    t->burst = b;

    return t;
}

/* Returns void pointer to initial list of tasks
to be provided to the algorithms. These are read
from a file provided as a parameter */
void *build_tlist(char *p) {

    char t_list[30][10];
    char r_list[30][10];
    int i = 0;
    FILE *t_file = fopen(p, "r");
    node *p_head = NULL;

    if(t_file == NULL) {
        perror("[-] ERROR: Unable to open file");
        exit(-1);
    }

    while(fgets(t_list[i], sizeof(t_list), t_file) != NULL) {
        t_list[i][strlen(t_list[i]) - 1] = '\0';
        i++;
    }

    // Generate mask of size i to access the array pseudorandomly
    int *mask = rand_index_list(i);

    for(int c = 0; c < i; c++) {

        if(strlen(t_list[mask[c]]) >= 10) {
            printf("[-] ERROR: String too long");
            exit(-1);
        }

        task *t = build_task(t_list[mask[c]]);
        push(&p_head, t);


    }

    return p_head;
}

/* Generates list of random numbers of provided size in order to use as random mask when pushing
to the queue, this is to simulate tasks arriving in different orders mainly for FCFS queues. */
int *rand_index_list(unsigned int size) {

    // Initialize index list, time type and seed rand() using time()
    static int i_list[50];
    time_t t;
    srand((unsigned) time(&t));

    // Generate list size long
    for(int i = 0; i < size; i++) {
        i_list[i] = i;
    }

    // Shuffle list using mod of rand() and the max size
    for(int i = 0; i < size; i++) {
        int tmp = i_list[i];
        int r_index = rand() % size;

        i_list[i] = i_list[r_index];
        i_list[r_index] = tmp;
    }

    return i_list;
}

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("[-] Not enough parameters, please provide algorithm and path to task file.\n");
        printf("Usage: %s <algorithm> <path to file>\n", argv[0]);
        printf("Options:\n- fcfs\n- sjf\n- rr\n- ps\n- prr");
    }

    void *t_list = build_tlist(argv[2]);
    if(strcmp(argv[1], "fcfs") == 0) {
        schedule(t_list, 0);
    } else {
        
    }
}