#include <stdio.h>
#include <stdlib.h>

#include "freedom.h"

#include "clock.h"
#include "delay.h"
#include "led.h"
#include "uart.h"

#define USE_STDIO 0
#define USE_MALLOC 0

static inline void UART_BLINK() {
#if USE_MALLOC
  void *p = malloc(100);
#else
  void *p = 0;
#endif
#if USE_STDIO
  printf("Blink...%p\r\n", p);
#else
  uart_write("Blink...", 8);
  uart_write_hex((unsigned long int)p);
  uart_write("\r\n", 2);
#endif
#if USE_MALLOC
  free(p);
#endif
}

int main(void) {
  init_clocks();
  init_led_io();
  uart_init(115200);

  unsigned int pattern;
  for (;;) {
    pattern = 0b0111110001010100;
    while (pattern) {
      UART_BLINK();
      RGB_LED(pattern & 0b100 ? 100 : 0, pattern & 0b010 ? 100 : 0,
              pattern & 0b001 ? 100 : 0);
      delay(800);
      RGB_LED(0, 0, 0);
      delay(200);
      pattern >>= 3;
    }
  }
}
