#include "../include/csapp.h"
#include <stdlib.h>
unsigned int snooze(unsigned int secs);
/* SIGINT handler */
void sigint_handler(int sig) {
  printf("Caught SIGINT!\n");
  return;
}

int main(int argc, char *argv[]) {
  int seconds;
  sscanf(argv[1], "%u", &seconds);
  if (argc < 2) {
    unix_error("Usage: snooze seconds_to_sleep");
  }
  /* Install the SIGINT handler */
  if (signal(SIGINT, sigint_handler) == SIG_ERR)
    unix_error("signal error");
  snooze(seconds);
  return 0;
}
unsigned int snooze(unsigned int secs) {
  unsigned int rc;
  if ((rc = sleep(secs)) < 0) {
    unix_error("Sleep error");
  }
  printf("Slept for %d of %d secs\n", secs - rc, secs);
  return rc;
}
