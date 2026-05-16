#include "main.hpp"
import indicator;

extern "C" void SystemClock_Config(void);

void NON_BLOCKING_Delay(uint32_t* static_timestamp, uint32_t time, const std::function<void()>& func) {
  if (HAL_GetTick() - *static_timestamp > time) {
    *static_timestamp = HAL_GetTick();
    func();
  }
}

void NON_BLOCKING_Delay(uint32_t* static_timestamp, uint32_t time, const std::function<void(void)>& func);
// 此宏不可在成员方法中调用，因为静态局部变量不属于独立对象
#define NON_BLOCKING_DELAY(time, func)                                                                                 \
  do {                                                                                                                 \
    static uint32_t timeout_##__COUNTER__ = 0;                                                                         \
    NON_BLOCKING_Delay(&timeout_##__COUNTER__, time, func);                                                            \
  } while (0)

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
    NON_BLOCKING_DELAY(500, Indicator);
  }
}