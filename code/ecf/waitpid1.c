#include "../include/csapp.h"
#define N 2
int main() {
  int status, i;
  pid_t pid;

  /* Parent creates N children */
  for (i = 0; i < N; i++)
    /* Child */
    if ((pid = Fork()) == 0)
      exit(100 + i);
  /* Parent reaps  */
  while ((pid = waitpid(-1, &status, 0)) > 0) {
    if (WIFEXITED(status))
      printf("child %d terminated normally with exit status=%d\n", pid,
             WEXITSTATUS(status));
    else
      printf("child %d terminated abnormaly\n", pid);
    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)
      unix_error("waitpid error");
    exit(0);
  }
}
