#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


char* malloc_atoi(int val);
int strlen_int(int val);


int main(void) {
  int val = 0;
  
  char* int_str = malloc_atoi(val); 
  printf("%s\n", int_str);

  return 0;
}


char* malloc_atoi(int val) {
  int i = 0;
  bool positive = true;

  int len = strlen_int(val);
  char* str = calloc(len, sizeof(char*));

  if (val < 0) {
    val = -val;
    positive = false;
    str[0] = '-';
    i++;
  }

  while (i < len) {
    str[len - i - (int) positive] = val % 10 + '0';
    val /= 10;
    i++;
  }
  
  str[i] = '\0';

  return str;
}

int strlen_int(int val) {
  int len = 0;
  if (val < 0) {
    len++;
  }


  do {
    val /= 10;
    len++;
  } while (val != 0);
  return len;
}