#include "../include/csapp.h"
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  struct in_addr inaddr; /* addr in network byte order */
  unsigned int addr;     /* addr in host byte order */
  if (argc != 2) {
    unix_error("usage: hex2dd <hex number>\n");
  }
  /* Accpets an ip address as user input */
  sscanf(argv[1], "%x", &addr);
  /* Convert the ip address to a readable network addres */
  inaddr.s_addr = htonl(addr);
  /* Prints out the result */
  printf("%s\n", inet_ntoa(inaddr));
  exit(0);
}
