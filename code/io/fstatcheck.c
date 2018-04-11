#include "../include/csapp.h"
int main(int argc, char **argv) {
  struct stat fstat;
  char *type, *readok;
  int fd = atoi(argv[1]);
  Fstat(fd, &fstat);
  if (S_ISREG(fstat.st_mode))
    type = "Regular";
  else if (S_ISDIR(fstat.st_mode))
    type = "directory";
  else
    type = "other";
  if ((fstat.st_mode & S_IRUSR)) /* check read access */
    readok = "yes";
  else
    readok = "no";
  printf("type:%s,read:%s\n", type, readok);
  exit(0);
}
