
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// Returns void pointer to initial list of tasks
// to be provided to the algorithms. These are read
// from a file provided as a parameter
void *gather_tasks(char *p) {

    FILE *task_f = fopen(p, "r");
    char task_line[256];

    if(task_f == NULL) {
        perror("[-] ERROR: Unable to open file");
        exit(-1);
    }

    while(fgets(task_line, sizeof(task_line), task_f) != NULL) {
        
    }

}