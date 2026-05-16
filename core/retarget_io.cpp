#include "usart.h"
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

extern "C" {
#define huart_printf huart1

/**
 * LLVM-libc printf
 * 不同于 microlib / newlib / picolibc.
 * write 接管 stdout(1) 和 stderr(2)
 * isatty 声明交互式终端
 * fstat 声明 stdout/stderr 为字符设备
 * lseek 移动流指针报错
 */

#if 0
static void stdout_putchar(uint8_t ch) {
  while ((huart_printf.Instance->SR & UART_FLAG_TXE) == 0);
  huart_printf.Instance->DR = ch;
}
ssize_t write(int fd, const void* buf, size_t count) {
  if (fd == 1 || fd == 2) {
    const uint8_t* p = static_cast<const uint8_t*>(buf);
    for (size_t i = 0; i < count; i++) {
      if (p[i] == '\n') stdout_putchar('\r');
      stdout_putchar(p[i]);
    }
    return count;
  }
  return -1;
}
#else
ssize_t write(int fd, const void* buf, size_t count) {
  if (fd == 1 || fd == 2) {
    HAL_UART_Transmit(&huart_printf, (const uint8_t*)buf, count, HAL_MAX_DELAY);
    return count;
  }
  return -1;
}
#endif

int isatty(int fd) {
  if (fd == 0 || fd == 1 || fd == 2) {
    return 1;
  }
  return 0;
}

off_t lseek(int fd, off_t offset, int whence) {
  (void)fd;
  (void)offset;
  (void)whence;
  errno = ESPIPE;
  return -1;
}

int fstat(int fd, struct stat* st) {
  if (fd == 0 || fd == 1 || fd == 2) {
    st->st_mode = S_IFCHR;
    return 0;
  }
  return -1;
}

ssize_t read(int fd, void* buf, size_t nbyte) {
  (void)fd;
  (void)buf;
  (void)nbyte;
  return -1;
}
int close(int __fildes) {
  (void)__fildes;
  return -1;
}

ssize_t _write(int fd, const void* buf, size_t count) { return write(fd, buf, count); }
int _isatty(int fd) { return isatty(fd); }
off_t _lseek(int fd, off_t offset, int whence) { return lseek(fd, offset, whence); }
int _fstat(int fd, struct stat* st) { return fstat(fd, st); }
ssize_t _read(int fd, void* buf, size_t nbyte) { return read(fd, buf, nbyte); }
int _close(int __fildes) { return close(__fildes); }
} // extern "C"
