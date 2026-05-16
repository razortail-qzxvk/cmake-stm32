#include "main.hpp"
extern "C" void SystemClock_Config(void);

import indicator;

template <auto AnonymousTag = [] {}, typename F>
void NON_BLOCKING_Delay(uint32_t time, F&& func) {
  static uint32_t timestamp = HAL_GetTick();
  if (HAL_GetTick() - timestamp >= time) {
    timestamp = HAL_GetTick();
    std::forward<F>(func)();
  }
}

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
  puts("Hello LLVM-libc via UART!");
  while (true) {
    NON_BLOCKING_Delay(500, Indicator);
  }
}