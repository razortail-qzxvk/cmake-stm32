#include "usart.h"
#include <stddef.h>
#include <stdint.h>
#include <stm32f4xx_hal_uart.h>
#include <utility>

// clang-format off

extern "C" {

#define huart_printf huart3

/**
 * LLVM-libc
 * 完全不同于 microlib(ARMCC) / newlib / Picolibc
 * 无需关心 write / _write / fputc / FILE* 等等
 * 参见：
 * https://llvm.org/devmtg/2024-10/slides/lightning/Smith-Using-llvm-libc.pdf
 * https://llvm.org/devmtg/2024-10/slides/techtalk/Hosek-ModernEmbeddedDevelopment-with-LLVM.pdf
 * 
 * 关于 __llvm_libc_stdio_write/read 的返回值.
 * 上面的文档给出的是 ssize_t, 但它们又砍掉了所有POSIX相关的东西, 我们又用不了, 可以说是自相矛盾了. 所以这里就用int.
 */

struct __llvm_libc_stdio_cookie { UART_HandleTypeDef* uart = &huart_printf; } __llvm_libc_stdin_cookie , __llvm_libc_stdout_cookie, __llvm_libc_stderr_cookie;

int __llvm_libc_stdio_write(void* cookie, const char* buf, size_t size) {
  if (cookie == nullptr || buf == nullptr) return -1;
  auto* stream = static_cast<__llvm_libc_stdio_cookie*>(cookie);
  if (stream->uart == nullptr) return -1;
  if (!std::in_range<int>(size)) return -1;
  int size_ = static_cast<int>(size);
#if 0
  int transferred = 0;
  while (transferred < size_) {
    int remaining = size_ - transferred;
    uint16_t chunk = remaining > UINT16_MAX ? UINT16_MAX : static_cast<uint16_t>(remaining);
    if (HAL_UART_Transmit(stream->uart, reinterpret_cast<const uint8_t*>(buf + transferred), chunk, HAL_MAX_DELAY) != HAL_OK) {
      return transferred == 0 ? -1 : transferred;
    }
    transferred += chunk;
  }
  return transferred;
#else
  for (int i = 0; i < size_; ++i) {
    if (buf[i] == '\n') {
      while ((stream->uart->Instance->SR & UART_FLAG_TXE) == 0);
      stream->uart->Instance->DR = '\r';
    }
    while ((stream->uart->Instance->SR & UART_FLAG_TXE) == 0);
    stream->uart->Instance->DR = static_cast<uint8_t>(buf[i]);
  }
  return size_;
#endif
}

int __llvm_libc_stdio_read([[maybe_unused]] void *cookie, [[maybe_unused]] char *buf, [[maybe_unused]] size_t size) {
  return 0;
}

} // extern "C"
