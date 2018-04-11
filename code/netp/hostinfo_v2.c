#include "../include/csapp.h"
int main(int argc, char *argv[]) {
  struct addrinfo *p, *listp, hints;
  struct sockaddr_in *sockp;
  char buf[MAXLINE];
  int rc, flags;
  if (argc != 2) {
    fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
    exit(0);
  }
  /* Get a list of addrinfo records */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;       /* IPV4 only */
  hints.ai_socktype = SOCK_STREAM; /* Connection only */
  if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    exit(1);
  }

  /* Walk the list and display each IP address */
  for (p = listp; p; p = p->ai_next) {
    // CSAPP Page 653
    sockp = (struct sockaddr_in *)p->ai_addr;
    inet_ntop(AF_INET, &(sockp->sin_addr), buf, MAXLINE);
    printf("%s\n", buf);
  }
  /* Clean up */
  Freeaddrinfo(listp);
  exit(0);
}
