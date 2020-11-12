/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-06     yangjie      the first version
 */
#include "gd32vf103.h"
#include <rtthread.h>

int main(void)
{
    /* enable the LED clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* configure LED GPIO port */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

    gpio_bit_reset(GPIOC, GPIO_PIN_13);

    while(1){
        /* insert 500 ms delay */
        rt_thread_mdelay(500);
        /* toggle the LED */
        gpio_bit_write(GPIOC, GPIO_PIN_13, (bit_status)(1-gpio_input_bit_get(GPIOC, GPIO_PIN_13)));

        /* insert 500 ms delay */
        rt_thread_mdelay(500);
    }
}
