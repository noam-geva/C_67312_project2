//
// Created by oded on 11/08/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // required for strtok

#define BUFFER_SIZE 100 // number of characters to read in each fgets call
#define DELIMITERS " ~" // these are the delimiters for strtok - the characters to split by (space and tilde)
#define FILE_NOT_FOUND_ERR "ERROR: File not found!\n"
#define USAGE_MSG "ERROR: not enough arguments!\nUsage: strtokExample <path to file>\n"
#define FILE_OPEN_MODE "r"

int main(int argc, char *argv[]){
  if (argc < 2){
    fprintf(stderr, USAGE_MSG);
    return EXIT_FAILURE;
  }

  FILE *fd = fopen(argv[1], FILE_OPEN_MODE); // open the given file in read mode
  if (fd == NULL){ // check the file opened correctly
    fprintf(stderr, FILE_NOT_FOUND_ERR);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE] = {0}, *token = NULL;
  while (fgets(buffer, BUFFER_SIZE, fd) != NULL){
    token = strtok(buffer, DELIMITERS); // set the string to parse, read first token
    printf("%s-", token);
    // continue reading the last string put into strtok
    while ((token = strtok(NULL, DELIMITERS))){
      // continue to parse until there is nothing left to parse in buffer - that's when strtok will return NULL
      printf("%s-", token); // print tokens with a hyphen (-) separating them
    }
  }
  // Notice the new line printed between the lines - how did it get there?
  // what should you change to make sure there is no new line?
  return EXIT_SUCCESS;
}