#include "../include/csapp.h"
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct in_addr inaddr; /* add in network byte order */
  char *src;             /* dddr in host byte order */
  unsigned int addr;
  if (argc != 2) {
    unix_error("usage: dd2hex <ip address>");
  }
  if (inet_aton(argv[1], &inaddr) == 0) {
    unix_error("convert Internet host address failed");
  }
  addr = ntohl(inaddr.s_addr);
  printf("0x%x\n", addr);
}
