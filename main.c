#include "scheduler.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("[-] Not enough parameters, please provide algorithm and path to task file.\n");
        printf("Usage: %s <algorithm> <path to file>\n", argv[0]);
        printf("Options:\n- fifs\n- sjf\n- rr\n- ps\n- prr");
    }

    //void *ptr = gather_tasks(argv[2]);

    int *ptr = rand_index_list(10);
}