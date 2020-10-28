#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* dsubstr_replace_all(char* orig, char* search, char* replace) {

  int size_delta = strlen(replace) - strlen(search);

  char* new_str = orig;
  char* ss_ptr = strstr(new_str, search);
  
  while (ss_ptr != NULL) {
    new_str = calloc(strlen(orig) + size_delta + 1, sizeof(char));
    strncpy(new_str, orig, ss_ptr - orig);
    strcat(new_str, replace);
    ss_ptr = ss_ptr + strlen(search);
    strcat(new_str, ss_ptr);
    free(orig);
    orig = new_str;
    ss_ptr = strstr(orig, search);
  }

  return new_str;

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
