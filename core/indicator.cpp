#include "main.h"
#include "main.hpp"

void NON_BLOCKING_Delay(uint32_t* static_timestamp, uint32_t time, const std::function<void()>& func) {
  if (HAL_GetTick() - *static_timestamp > time) {
    *static_timestamp = HAL_GetTick();
    func();
  }
}

void Indicator() {
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  puts("Hello World.");
}