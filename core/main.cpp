#include "main.hpp"
import test;

extern "C" void SystemClock_Config(void);
extern void Indicator();

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
  puts("Hello LLVM-libc via UART!");
  while (true) {
    NON_BLOCKING_DELAY(500, Indicator);
  }
}