
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "freedom.h"
#include "uart.h"

int _close(int file) { return -1; }
int _isatty(int file) { return 1; }
int _open(const char *name, int flags, int mode) { return -1; }
off_t _lseek(int fd, off_t offset, int whence) { return 0; }
int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}
void _exit(int status) {
  for (;;) {
  }
}

int puts(const char *string) {
  return uart_write((char *)string, strlen(string));
}

int _write(int file, char *p, int len) {
  switch (file) {
  case 1:
    return uart_write(p, len); // stdout
  case 2:
    return uart_write_err(p, len); // stderr
  default:
    return -1;
  }
}

static char *heap_end = (char *)__heap_start;

char *_sbrk(int incr) {
  heap_end += incr; // TODO:  check for collisions with the stack
  return (heap_end - incr);
}

void init_stdio() {}
