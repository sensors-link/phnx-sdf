#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "system.h"
#include "uart.h"

static int __putc(int ch) {
	
#if defined(_UART2_PRINT)
    UART2 -> THR = ch;
    while( (UART2->LSR & (1ul<<6)) == 0) {};	
#else
    UART1 -> THR = ch;
    while( (UART1->LSR & (1ul<<6)) == 0) {};
#endif
	// return ch;	
	return 1;	
}

static int __getc() {
	char ch;

#if defined(_UART2_PRINT)
    while( (UART2->LSR & (1ul<<0)) == 0);
    ch = UART2 -> RBR & 0xff;	
#else
    while( (UART1->LSR & (1ul<<0)) == 0);
    ch = UART1 -> RBR & 0xff;	
#endif
	return ch;
}

static int uart_write(char *p, int len)
{
    for(int i=0;i<len;i++) {
        __putc(p[i]);
    }
    return len;
}

#define uart_write_err(p,l) uart_write((p),(l))

static int uart_read(char *p, int len) {
    int i, c;
    for(i=0;i<len;i++) {
        c = __getc();
        if( c < 0 ) break;
        p[i] = (char) c;
    }
    return i;
}

#undef errno
int errno;
int _close(int file) { return -1; }
int _isatty(int file) { return 1; }
int _open(const char *name, int flags, int mode) { return -1; }
off_t _lseek(int fd, off_t offset, int whence) { return 0;}
int _fstat(int file, struct stat *st) {st->st_mode = S_IFCHR; return 0;}
void _exit(int status) { for(;;){} }

int _write(int file, char *p, int len)
{
    switch(file) {
     case 1:        return uart_write(p, len);              // stdout
     case 2:        return uart_write_err(p,len);           // stderr
     default:       return -1;
    }
}

int _read(int file, char *p, int len)
{
    return uart_read(p, len);
}

// ------------------------------------------------------------------------------------
// _sbrk(len) -- Allocate space on the heap

static char *heap_end = (char *) __heap_start;

char *_sbrk(int incr)
{
    heap_end += incr;                   // TODO:  check for collisions with the stack
    return (heap_end - incr);
}
