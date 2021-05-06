#ifndef DRIVER_
#define DRIVER_

typedef struct task {
    unsigned int id;
    unsigned int weight;
    unsigned int burst;
} task;

// Reads list of tasks from provided file
void *gather_tasks(char *p);
int *rand_index_list(unsigned int size);

#endif