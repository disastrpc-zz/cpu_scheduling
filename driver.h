#ifndef DRIVER_
#define DRIVER_

typedef struct task {
    unsigned int id;
    unsigned int weight;
    unsigned int burst;
    unsigned int resp;
    unsigned int turn;
    unsigned int wait;
} task;

// Reads list of tasks from provided file
void *build_tlist(char *p);
int *rand_index_list(unsigned int size);

#endif