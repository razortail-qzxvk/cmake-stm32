#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <numeric>
#include <queue>
#include <stdio.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

void NON_BLOCKING_Delay(uint32_t* static_timestamp, uint32_t time, const std::function<void(void)>& func);
// 此宏不可在成员方法中调用，因为静态局部变量不属于独立对象
#define NON_BLOCKING_DELAY(time, func)                                                                                                                                                                                                                                                                     \
  do {                                                                                                                                                                                                                                                                                                     \
    static uint32_t timeout_##__COUNTER__ = 0;                                                                                                                                                                                                                                                             \
    NON_BLOCKING_Delay(&timeout_##__COUNTER__, time, func);                                                                                                                                                                                                                                                \
  } while (0)

// extern uint8_t UART1_RxBuf[256+1];
// extern uint8_t UART6_RxBuf[256+1];