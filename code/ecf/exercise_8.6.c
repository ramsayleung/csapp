#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *envp[]) {
  printf("Command-ine arguments:\n");
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]:%s\n", i, argv[i]);
  }
  printf("Environment variables:\n");
  int index = 0;
  while (envp[index++] != NULL) {
    printf("envp[%d]:%s\n", index - 1, envp[index - 1]);
  }
}
