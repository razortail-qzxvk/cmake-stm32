#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include <cstdint>

import test;

extern "C" void SystemClock_Config(void);

int main() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM14_Init();
  MX_SPI3_Init();
  [[maybe_unused]] int sum = add(3, 4);
  while (true) {
  }
}