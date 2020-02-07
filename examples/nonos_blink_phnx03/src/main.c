#include "gd32vf103.h"
#include <stdio.h>

void delay_1ms(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;

    // Don't start measuruing until we see an mtime tick
    uint64_t tmp = get_timer_value();
    do {
    start_mtime = get_timer_value();
    } while (start_mtime == tmp);

    do {
    delta_mtime = get_timer_value() - start_mtime;
    }while(delta_mtime <(SystemCoreClock/4000.0 *count ));
}

extern void init_libc();
extern void init_stdio();

int main(void)
{
    init_libc();
    init_stdio();
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* configure led GPIO port */ 
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); /* BUILTIN LED OF LONGAN BOARDS IS PIN PC13 */

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 57600U); 
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    GPIO_BC(GPIOC) = GPIO_PIN_13;

    // printf("a usart transmit test example!\n");
    while(1) {
        printf("Blink...\r\n");
        /* turn on builtin led */
        GPIO_BOP(GPIOC) = GPIO_PIN_13;
        delay_1ms(1000);
        /* turn off uiltin led */
        GPIO_BC(GPIOC) = GPIO_PIN_13;
        delay_1ms(1000); 
    }
}
