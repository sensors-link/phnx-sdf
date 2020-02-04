#include <stdio.h>

#include "freedom.h"

#include "clock.h"
#include "delay.h"
#include "led.h"
#include "uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void startupBlink(void) {
  // Blink the green LED to indicate booting
  unsigned int pattern = 0b1100110011001100;
  while (pattern) {
    RGB_LED(0, pattern & 1 ? 100 : 0, 0); // Set GREEN led based on LSB
    pattern >>= 1;
    delay(25);
  }
}

static void taskRBlink(void *param) {
  const TickType_t xDelay = 500; // ms
  int x = 0;
  for (;;) {
    RED_LED(x);
    x = 50 - x;
    vTaskDelay(xDelay / portTICK_PERIOD_MS);
  }
}

/*-----------------------------------------------------------*/

static void taskGBlink(void *param) {
  const TickType_t xDelay = 200; // ms
  int x = 0;
  for (;;) {
    GREEN_LED(x);
    x = 50 - x;
    vTaskDelay(xDelay / portTICK_PERIOD_MS);
  }
}

/*-----------------------------------------------------------*/

static void taskBBlink(void *param) {
  const TickType_t xDelay = 2000; // ms
  int x = 0;
  for (;;) {
    BLUE_LED(x);
    x = 50 - x;
    vTaskDelay(xDelay / portTICK_PERIOD_MS);
  }
}

/*-----------------------------------------------------------*/
#define USE_STDIO 0
static void taskUBlink(void *param) {
  const TickType_t xDelay = 1000; // ms
  for (;;) {
#if USE_STDIO
    printf("Timer Expired!\r\n");
#else
    uart_write("Timer Expired!\r\n", 16);
#endif
    vTaskDelay(xDelay / portTICK_PERIOD_MS);
  }
}

int main(void) {
  init_clocks();
  init_led_io();
  uart_init(115200);

  startupBlink();

  // Test FreeRTOS Tasks
  xTaskCreate(taskRBlink, "RBlink", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
  xTaskCreate(taskGBlink, "GBlink", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  xTaskCreate(taskBBlink, "BBlink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(taskUBlink, "UBlink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();

  // Should not reach here.
  for (;;) {
  }
}
