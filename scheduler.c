
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"

// Returns void pointer to initial list of tasks
// to be provided to the algorithms. These are read
// from a file provided as a parameter
void *gather_tasks(char *p) {

    FILE *t_file = fopen(p, "r");
    char t_line[30][10];
    int i = 0;

    if(t_file == NULL) {
        perror("[-] ERROR: Unable to open file");
        exit(-1);
    }

    while(fgets(t_line[i], sizeof(t_line), t_file) != NULL) {
        t_line[i][strlen(t_line[i]) - 1] = '\0';
        i++;
    }

    for(int c = 0; c < i; c++) {
        printf("%s\n", t_line[c]);
    }

    return NULL;
}

int *rand_index_list(unsigned int size) {

    int i_list[size];
    int *p_list = i_list;
    time_t t;
    srand((unsigned) time(&t));

    for(int i = 0; i < size; i++) {
        i_list[i] = i;
    }

    for(int i = 0; i < size; i++) {
        int tmp = i_list[i];
        int r_index = rand() % size;

        i_list[i] = i_list[r_index];
        i_list[r_index] = tmp;
    }

    for(int i = 0; i < size; i++) {
        printf("%d\n", i_list[i]);
    }

    return NULL;
}