#include "../include/csapp.h"
unsigned int snooze(unsigned int secs) {
  unsigned int rc;
  if ((rc = sleep(secs)) < 0) {
    unix_error("Sleep error");
  }
  printf("Slept for %d of %d secs", secs - rc, secs);
  return rc;
}
