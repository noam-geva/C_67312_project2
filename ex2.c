#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex2.h"

// magic const
#define MAX  200
#define INVALID_POINTER "the given pointer is invalid.\n"
#define COULD_NOT_ALLOCATE "couldn't allocate data.\n"

/**
 * helper function that return a single nodes average.
 * gets a valid node from GetAverages.
 * will never be called if node->len = 0.
 * @param node.
 * @return double = ave.
 */
double single_node_average(Node *node){
    double sum = 0; //initialize
    for (unsigned int ix = 0; ix < node->len; ix++){
        sum += node->data[ix];
    }
    return sum/(node->len);
}

#define CHECK_ALLOC(alloc_ptr, free_ptr1, free_ptr2) if (!check_alloc(alloc_ptr, free_ptr1, free_ptr2)) { return NULL; }
void *check_alloc(void *alloc_ptr, void *free_ptr1, void *free_ptr2){
    if (alloc_ptr == NULL){
        free(free_ptr1);
        free(free_ptr2);
        fprintf(stderr, COULD_NOT_ALLOCATE);
        return NULL;
    } else {
        return alloc_ptr;
    }

}

/**
 *  @TODO dynamically allocates an array that will need to be free in FreeLinkedList
 * @param line of number with out e/s.
 * @return returns the Node
 */
Node *CreateNode(char *line){
    Node *ret_val = calloc(1,sizeof(Node));// initialize a return value
    // start of conversion from str to Lint
    CHECK_ALLOC(ret_val,NULL,NULL);
    char *token = strtok(line,",");
    int curr_data = strtol(token, NULL, 10);
    int *data_ptr = malloc(sizeof(int));
    CHECK_ALLOC(data_ptr,ret_val,NULL);
    int *old_data_ptr = data_ptr;
    if (data_ptr == NULL){return NULL;}
    data_ptr[0] = curr_data;
    int num_of_elem = 1; //first declaration in the line above make that so we wont count the first number
    while ((token = strtok(NULL, ",")) != NULL){
        num_of_elem ++;
        old_data_ptr = data_ptr;
        data_ptr = realloc(data_ptr, num_of_elem*sizeof(int));
        CHECK_ALLOC(data_ptr,ret_val,old_data_ptr);
        curr_data = strtol(token, NULL, 10);
        data_ptr[num_of_elem-1] = curr_data;
    }
    // end of conversion from str to Lint
    ret_val->len = num_of_elem;
    ret_val->data = data_ptr;
    return ret_val;
}

LinkedList *CreateList(){
    LinkedList *list = malloc(sizeof(struct LinkedList));
    CHECK_ALLOC(list,NULL,NULL);
    list->head = NULL;
    list->tail = NULL;
    return list;

}

/**
 * Adds a node as the head of the list
 * Assumptions:
    * You cannot assume the pointers are valid

 * In case of errors:
    * Invalid pointer - print error to stderr and return from function.

 * @param list pointer to list to add a node to
 * @param node pointer to the node to add to the list
 */

void AddToStartLinkedList(LinkedList *const list, Node *const node){
    if (node == NULL){
        fprintf(stderr,INVALID_POINTER);
        return;
    }
    if (list->head == NULL){
        list->head = node;
        list->tail = node;
        return;
    }
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

/**
 * Adds a node as the tail of the list
 *
 * Assumptions:
    * You cannot assume the pointers are valid

 * In case of errors:
    * Invalid pointer - print error to stderr and return from function.

 * @param list pointer to list to add a node to
 * @param node pointer to the node to add to the list
 */
void AddToEndLinkedList(LinkedList *const list, Node *const node) {
    if (node == NULL){
        fprintf(stderr, INVALID_POINTER);
        return;
    }
    if (list->tail == NULL){
        list->tail = node;
        list->head = node;
        return;
    }
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
}


/**
 * removes a node from the list and frees it's resources
 *
 * Assumptions:
    * You cannot assume the pointers are valid
    * You cannot assume the node is in the list. In this case don't free its resources.
    * To be sure - you are supposed to compare the given node to the nodes in the list using == operator

 * In case of errors:
    * Invalid pointer - print informative error message to stderr and return from function.

 * @param list pointer to list to remove a node from
 * @param node pointer to the node to remove from the list
 */
void RemoveNode(LinkedList *const list, Node *const node){
    // input check
    if (node == NULL || list == NULL){
        fprintf(stderr,INVALID_POINTER);
        return;
    }
    if (list->head == list->tail){ //we may assume a non circular list
        if (list->head == node){
            list->head = NULL;
            list->tail = NULL;
            free(node->data);
            free(node);
        }
        return;
    }
    Node *curr_node = list->head;
    int node_not_in_list = 1;
    while (curr_node){
        if (curr_node == node){
            node_not_in_list = 0; // node is in the list (not not)
            break;
        }
        curr_node = curr_node->next;
    }
    if (node_not_in_list == 1){return;}
    //if node in mid list
    if (node->next && node->prev){
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    if (node->next){ // node is head
        node->next->prev = NULL;
        list->head = node->next;
    }
    if (node->prev){ // node is tail
        node->prev->next = NULL;
        list->tail = node->prev;
    }
    free(node->data); // free node's data
    free(node); // free node it self (wont be able to free it in list because its not linked anymore
}


/**
 * Frees the resources (all dynamic allocations) of the given list.
 *
 * Assumptions:
    * You cannot assume the pointer is valid

 * In case of errors:
    * Invalid pointer - This means there is nothing to free, just return from function.
    * Node is not in the list - Return from the function. Don't free its resources!

 * @param list the list to free.
 */
void FreeLinkedList(LinkedList *const list){
    if(list == NULL){
        return;
    }
    Node *curr = list->tail;
    // while loop will free all but the head
    while (curr->prev != NULL){
        curr =curr->prev;
        free(curr->next->data);
        free(curr->next);
    }
    free(list->head->data);
    free(list->head);
    free(list);
}

/**
 * calculates the grade average of every node in the linked list
 * @param list: LinkedList pointer to linked list from whom to calculate grade averages
 * @param num_elements_in_returned_array: pointer to unsigned long, that should contain the number of nodes in the list
 * (for nodes with data != NULL) when the function finishes execution. To be clear - at the start of the function,
 * (*num_elements_in_returned_array) is a garbage value, and this function should set its value.
 * Note: Nodes with NULL data should not be count in the calculation. Therefore *num_elements_in_returned_array <= numOfNodes.
 *
 * Assumptions:
    * You cannot assume the pointers are valid
    * You don't know the number of nodes in the linkedlist in advance
    * You can assume that there is at least one node with data != NULL (i.e. len > 0)
    * You can assume that for each node 'len' is indeed the length of 'data'
    *  (in particular len=0 <=> data=NULL).

 * In case of errors:
    * Invalid pointer - print informative message to stderr, free resources allocated by function,
                        return NULL from function.
    * Allocation fail - print informative message to stderr, free resources allocated by function,
                        return NULL from function.
    * Node with NULL data - don't count the node in the calculation.
 * @return array of grade averages per node. To be clear - this means you should
 * calculate the grade average of every node and save it into an array, which you will return from the function
 */
double *GetAverages(LinkedList *const list, size_t *const num_elements_in_returned_array){
    // @TODO free memory!
    // list validity check
    if (list == NULL){
        fprintf(stderr, INVALID_POINTER);
        return NULL;
    }
    double *ret = NULL; // initialize return value
    double  *old_ret;
    unsigned int node_count = 0;
    Node *curr_node = list->head;
    while( curr_node != NULL) {
        if (curr_node->data) {
            node_count++;
            old_ret = ret;
            ret = realloc(ret, node_count*sizeof(double));
            // realloce validity check
            CHECK_ALLOC(ret,old_ret,NULL);
            ret[node_count-1] = single_node_average(curr_node);
        }
        curr_node = curr_node->next;
    }
    *num_elements_in_returned_array = node_count;
    return ret;
}


/**
 * opens a file from a given filename and parses it's contents into a LinkedList.
 *
 * Assumptions:
     * You cannot assume the filename is OK (meaning - you cannot assume it is a valid path to a file).
     * You cannot assume anything on the number of ints in a row of the file, just that it is >= 1
     * You can assume that if the file opened, then it is exactly in the format specified in the exercise PDF.
     * You can assume max line length of 1024.
 *
 * In case of errors:
    * Invalid pointer - print informative message to stderr, free resources allocated by function,
                        return NULL from function.
    * File open fail - print informative message to stderr, free resources allocated by function,
                        return NULL from function.
    * Allocation fail - print informative message to stderr, free resources allocated by function,
                        return NULL from function.
 * Think - in what mode should we open this file? (read? write? append?)
 *
 * @param file filename of input file that needs to be parsed
 * @return pointer to LinkedList instance, whose nodes and data arrays were parsed from the given file.
 */
LinkedList *ParseLinkedList(const char *const filename){
    //check file validity
    FILE *file = fopen(filename,"r");
    if (file == NULL){
        return NULL;
    }
    char curr_line[MAX]; // str declaration (for a single line in the file)
    LinkedList *list = CreateList(); // initialize a return value
    CHECK_ALLOC(list, NULL, NULL);
    while (fgets(curr_line, MAX ,file )){
        Node *curr_node = CreateNode(&curr_line[2*sizeof(char )]);
        if (curr_node == NULL){
            FreeLinkedList(list);
            fclose(file);
            CHECK_ALLOC(NULL,NULL,NULL);
        }
        if ( curr_line[0] == 'e'){
            AddToEndLinkedList(list,curr_node);
        }
        else{ // may assume a line starts with either an e or a s
            AddToStartLinkedList(list,curr_node);
        }
    }
    fclose(file);
    return list;
}
void PrintData(Node *node){
    for (int ix = 0; ix < node->len ; ix ++){
        printf("%i\t", node->data[ix]);
    }
    printf("\n");
}

void PrintList(struct LinkedList * list){
    if (list == NULL || list->head == NULL){printf("NULL");return;}
    printf("the head is:\t");
    PrintData(list->head);
    Node *curr_node = list->head;
    while (curr_node){
        PrintData(curr_node);
        curr_node = curr_node->next;
    }
    printf("the tail is:\t");
    PrintData(list->tail);
    printf("------------\n");
}
