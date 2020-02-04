#ifndef _LED_H_
#define _LED_H_

extern void init_led_io(void);
extern void fault(uint32_t pattern);
#define FAULT_FAST_BLINK (0b10101010101010101010101010101010)
#define FAULT_MEDIUM_BLINK (0b11110000111100001111000011110000)
#define FAULT_SLOW_BLINK (0b11111111000000001111111100000000)

static inline void RGB_LED(int red, int green, int blue)
{
    TPM2_C0V = red;
    TPM2_C1V = green;
    TPM0_C1V = blue;
}

#define RED_LED(x) (TPM2_C0V = x)
#define GREEN_LED(x) (TPM2_C1V = x)
#define BLUE_LED(x) (TPM0_C1V = x)

#endif
