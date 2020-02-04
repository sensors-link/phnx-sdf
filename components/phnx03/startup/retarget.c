
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#include "gd32vf103.h"

int _close(int file) { return -1; }
int _isatty(int file) { return 1; }
int _open(const char *name, int flags, int mode) { return -1; }
off_t _lseek(int fd, off_t offset, int whence) { return 0; }
int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}


int _put_char(int ch)
{
    usart_data_transmit(USART0, (uint8_t) ch );
    while (usart_flag_get(USART0, USART_FLAG_TBE)== RESET){
    }

    return ch;
}

ssize_t _write(int fd, const void* ptr, size_t len) {
	const uint8_t * current = (const uint8_t *) ptr;

  for (size_t jj = 0; jj < len; jj++) {
    _put_char(current[jj]);

    if (current[jj] == '\n') {
      _put_char('\r');
    }
  }
  return len;
}

size_t _read(int fd, void *ptr, size_t len) {
  return -1;
}

// static char *heap_end = (char *)__heap_start;
extern uint32_t _end[];
static char *heap_end = (char *)_end;

char *_sbrk(int incr) {
  heap_end += incr; // TODO:  check for collisions with the stack
  return (heap_end - incr);
}

void init_stdio() {}