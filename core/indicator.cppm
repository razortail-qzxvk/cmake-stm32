module;
#include "main.h"
#include "main.hpp"
#include <cstdint>
export module indicator;

export void Indicator() {
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  puts("Hello World.");
}
