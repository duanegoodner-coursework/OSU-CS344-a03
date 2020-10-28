#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {

  char orig[] = "orig$$$$in$$";
  char search[] = "$$";
  char replace[] = "1234";

  char* dyn_string_orig = calloc(strlen(orig) + 1, sizeof(char));
  strcpy(dyn_string_orig, orig);

  printf("Original: %s\n", dyn_string_orig);

  int size_delta = strlen(replace) - strlen(search);

  char* ss_ptr = strstr(dyn_string_orig, search);
  
  while (ss_ptr != NULL) {
    char* temp = calloc(strlen(dyn_string_orig) + size_delta + 1, sizeof(char));
    strncpy(temp, dyn_string_orig, ss_ptr - dyn_string_orig);
    strcat(temp, replace);
    ss_ptr = ss_ptr + strlen(search);
    strcat(temp, ss_ptr);
    free(dyn_string_orig);
    dyn_string_orig = temp;
    ss_ptr = strstr(dyn_string_orig, search);
  }

  printf("Modified: %s\n", dyn_string_orig);

  return 0;
} 