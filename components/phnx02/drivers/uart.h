#ifndef __UART_H__
#define __UART_H__

extern void UART2_IRQHandler() __attribute__((interrupt("IRQ")));
extern void UART1_IRQHandler() __attribute__((interrupt("IRQ")));
extern void UART0_IRQHandler() __attribute__((interrupt("IRQ")));
extern int uart_write(char *p, int len);
extern void uart_write_hex(unsigned long int hex);
extern int uart_write_err(char *p, int len);
extern int uart_read(char *p, int len);
extern void uart_init(int baud_rate);

#endif
