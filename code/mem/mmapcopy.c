#include "../include/csapp.h"

int main(int argc, char *argv[]) {
  struct stat stat;
  int fd;
  if (argc != 2) {
    printf("usage: %s <file_name>", argv[0]);
    exit(1);
  }
  /* Open file */
  fd = Open(argv[1], O_RDONLY, 0);
  Fstat(fd, &stat);
  char *bufp = Mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  Write(STDOUT_FILENO, bufp, stat.st_size);
  exit(0);
}
