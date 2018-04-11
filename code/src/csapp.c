#include "../include/csapp.h"
/**************************
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
/* Unix stype error */
void unix_error(char *msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

/* Posix-style error */
void posix_error(int code, char *msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

/* Getaddrinfo-style error */
void gai_error(int code, char *msg) {
  fprintf(stderr, "%s: %s\n", msg, gai_strerror(code));
  exit(0);
}
/* Application error */
void app_error(char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(0);
}
/* Obsolete gethostbyname error */
void dns_error(char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(0);
}

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) {
  ssize_t n;
  if ((n = rio_readn(fd, ptr, nbytes)) < 0)
    unix_error("Rio_readn error");
  return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) {
  if (rio_writen(fd, usrbuf, n) != n)
    unix_error("Rio_writen error");
}
void Rio_readinitb(rio_t *rp, int fd) { rio_readinitb(rp, fd); }

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
  ssize_t rc;
  if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
    unix_error("Rio_readnb error");
  return rc;
}
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  ssize_t rc;
  if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
    unix_error("Rio_readlineb error");
  return rc;
}

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
  while (nleft > 0) {
    if ((nread = read(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) /* Interrupted by sig handler return */
        nread = 0;        /* and call read() again */
      else
        return -1; /* errno set by read() */
    } else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
  }
  return (n - nleft); /* Return >=0 */
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = usrbuf;
  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR) /* Interrupted by sig handler return */
        nwritten = 0;
      else
        return -1;
    }
    nleft -= nwritten;
    bufp += nwritten;
  }
  return n;
}

void rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;
  rp->rio_bufptr = rp->rio_buf;
  rp->rio_cnt = 0;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
  int cnt;
  while (rp->rio_cnt <= 0) { /* Refill if buf is empty */
    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
    if (rp->rio_cnt < 0) {
      if (errno != EINTR) /* Interrupted by sig handler return */
        return -1;
    } else if (rp->rio_cnt == 0) /* EOF */
      return 0;
    else
      rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
  }
  /* Copy min(n,rp->rio_cnt) bytes from internal buf to user buf*/
  cnt = n;
  if (rp->rio_cnt < n)
    cnt = rp->rio_cnt;
  memcpy(usrbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;
  return cnt;
}
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  int n, rc;
  char c, *bufp = usrbuf;
  for (n = 1; n < maxlen; n++) {
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      *bufp++ = c;
      if (c == '\n') {
        n++;
        break;
      }
    } else if (rc == 0) {
      if (n == 1)
        return 0; /* EOF,no data read */
      else
        break; /* EOF, some data was read */
    } else
      return -1; /* ERROR */
  }
  *bufp = 0;
  return n - 1;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
  while (nleft > 0) {
    if ((nread = rio_read(rp, bufp, nleft)) < 0)
      return -1; /* errno set by read */
    else if (nread == 0)
      break; /* EOF */
    nleft -= nread;
    bufp += nread;
  }
  return (n - nleft); /* Return >=0 */
}

/********************************
 * Wrappers for Unix I/O routines
 ********************************/

int Open(const char *pathname, int flags, mode_t mode) {
  int rc;
  if ((rc = open(pathname, flags, mode)) < 0)
    unix_error("Open error");
  return rc;
}

ssize_t Read(int fd, void *buf, size_t count) {
  ssize_t rc;
  if ((rc = read(fd, buf, count)) < 0)
    unix_error("Read error");
  return rc;
}

ssize_t Write(int fd, const void *buf, size_t count) {
  ssize_t rc;
  if ((rc = write(fd, buf, count)) < 0)
    unix_error("Write error");
  return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) {
  off_t rc;
  if ((rc = lseek(fildes, offset, whence)) < 0)
    unix_error("Lseek error");
  return rc;
}

void Close(int fd) {
  int rc;
  if ((rc = close(fd)) < 0)
    unix_error("Close error");
}

int Dup2(int fd1, int fd2) {
  int rc;
  if ((rc = dup2(fd1, fd2)) < 0)
    unix_error("Dup2 error");
  return rc;
}
void Stat(const char *filename, struct stat *buf) {
  if (stat(filename, buf) < 0)
    unix_error("Stat error");
}
void Fstat(int fd, struct stat *buf) {
  if (fstat(fd, buf) < 0)
    unix_error("Fstat error");
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
char *Fgets(char *ptr, int n, FILE *stream) {
  char *rptr;
  if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
    app_error("Fgets error");
  return rptr;
}

/* Wrappers for Unix process control functions */
/* Fork */
pid_t Fork(void) {
  pid_t pid;
  if ((pid = fork()) < 0)
    unix_error("Fork error");
  return pid;
}

/* $begin kill */
void Kill(pid_t pid, int signum) {
  int rc;
  if ((rc = kill(pid, signum)) < 0)
    unix_error("Kill error");
}

/* $begin pause */
void Pause() {
  (void)pause();
  return;
}

/* $begin sleep */
unsigned int Sleep(unsigned int secs) {
  unsigned int rc;
  if ((rc = sleep(secs)) < 0)
    unix_error("Sleep error");
  return rc;
}

/*************************************************************
 * The Sio (Signal-safe I/O) package - simple reentrant output
 * functions that are safe for signal handlers.
 *************************************************************/

/* Private sio functions */
/* $begin sioprivate */
static void sio_reverse(char s[]) {
  int c, i, j;
  for (i = 0, j = strlen(s); i < j; i++, i--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/* sio_ltoa - Convert long to base b string(from K&R) */
static void sio_ltoa(long v, char s[], int b) {
  int c, i = 0;
  do {
    s[i++] = ((c = (v % b)) < 10) ? c + '0' : c - 10 + 'a';
  } while ((v /= b) > 0);
  s[i] = '\0';
  sio_reverse(s);
}
/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[]) {
  int i = 0;
  while (s[i] != '\0')
    ++i;
  return i;
}
/* Public Sio functions */
/* Put string */
ssize_t sio_puts(char s[]) { return write(STDOUT_FILENO, s, sio_strlen(s)); }

/* Put long */
ssize_t sio_putl(long v) {
  char s[128];
  sio_ltoa(v, s, 10); /* Based on K&R itoa() */
  return sio_puts(s);
}

/* Put error message and exit */
void sio_error(char s[]) {
  sio_puts(s);
  _exit(1);
}

handler_t *Signal(int signum, handler_t *handler) {
  struct sigaction action, old_action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask); /* Block sign of type being handled */
  action.sa_flags = SA_RESTART; /* Restart syscalls if possible */
  if (sigaction(signum, &action, &old_action) < 0)
    unix_error("Signal error");
  return (old_action.sa_handler);
}

/* Wrappers for the SIO routines */
ssize_t Sio_putl(long v) {
  ssize_t n;
  if ((n = sio_putl(v)) < 0)
    sio_error("Sio_putl error");
  return n;
}

ssize_t Sio_puts(char s[]) {
  ssize_t n;
  if ((n = sio_puts(s)) < 0)
    sio_error("Sio_puts error");
  return n;
}

void Sio_error(char s[]) { sio_error(s); }

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
  void *ptr;
  if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *)-1))
    unix_error("mmap error");
  return (ptr);
}

/*******************************
 * Protocol-independent wrappers
 *******************************/
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags) {
  int rc;
  if ((rc = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags)) != 0)
    gai_error(rc, "Getnameinfo error");
}

void Freeaddrinfo(struct addrinfo *res) { freeaddrinfo(res); }
