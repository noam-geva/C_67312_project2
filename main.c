#include <stdlib.h>
#include <stdio.h>
#include "ex2.h"
#include "ex2.c"

#define DOUBLE_2_FORMAT "%.2lf"
#define ARGC_ERROR_MSG "ERROR: Wrong number of arguments.\nUsage: main <path to file>\n"

// main to run functions from Ex2

int main(){
    printf("WARNING: These tests are very basic, and mostly have to do with re-linking your list correctly.\n"
           "These check the three basic functions: AddToStartLinkedList, AddToEndLinkedList, RemoveNode.\n");
    if (test1() || test2()){
        printf("Failed a test. Sorry :(\n");
    }
    else {
        printf("PASSED ALL TESTS!\nREMEMBER:\n1. Erase main, test1 and test2 functions.\n2. Use Valgrind! "
               "For example, FreeLinkedList was not tested; RemoveNode may still have allocated memory, etc.\n");
    }
    return 0;
}

