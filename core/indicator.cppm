module;
#include "main.h"
#include "main.hpp"
export module indicator;
import my_std;

export void Indicator() {
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  std::puts("Hello World.");
}
