#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void dsubstr_replace_all(char* orig, char* search, char* replace) {

  int size_delta = strlen(replace) - strlen(search);

  char* ss_ptr = strstr(orig, search);
  
  while (ss_ptr != NULL) {
    char* temp = calloc(strlen(orig) + size_delta + 1, sizeof(char));
    strncpy(temp, orig, ss_ptr - orig);
    strcat(temp, replace);
    ss_ptr = ss_ptr + strlen(search);
    strcat(temp, ss_ptr);
    free(orig);
    orig = temp;
    ss_ptr = strstr(orig, search);
  }
}

char* int_to_dynstr(int n) {
    // plenty of space for string. could use log & floor to just malloc
    // but using the quick & easy way for now.
    char string_buffer[100];

    sprintf(string_buffer, "%d", n);
    char* dyn_istring = calloc(strlen(string_buffer) + 1, sizeof(char));
    strcpy(dyn_istring, string_buffer);

    return dyn_istring;


}
