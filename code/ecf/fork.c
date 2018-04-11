#include "../include/csapp.h"
int main() {
  pid_t pid;
  int x = 1;
  pid = Fork();
  /* Child */
  if (pid == 0) {
    printf("child : x=%d\n", ++x);
  }
  /* Parent */
  printf("parent: x=%d\n", --x);
  exit(0);
}
