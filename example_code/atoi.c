#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void atoi(char* str, int len, int val);
int strlen_int(int val);


int main(void) {
  char str[100];
  int val = -20;
  int len = strlen_int(val);

  atoi(str, len, val);

  printf("%s\n", str);


  return 0;
}


void atoi(char* str, int len, int val) {
  int i = 0;
  bool positive = true;

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
}


int strlen_int(int val) {
  int len = 0;

  if (val < 0) {
    len++;
  }

  while (val != 0) {
    val /= 10;
    printf("\nval = %d\n", val);
    len++;
  }

  printf("\nlen = %d\n", len);

  return len;

}

