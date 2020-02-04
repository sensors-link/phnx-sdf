#include <stdint.h>
#include <stdlib.h>
// #include "gd32vf103.h"
#include "riscv_encoding.h"

extern int main();
extern void init_stdio();

extern uint32_t _data_lma[];
extern uint32_t _data[];
extern uint32_t _edata[];
extern uint32_t __bss_start[];
extern uint32_t _end[];

// extern void __libc_init_array();
// extern void __libc_fini_array();
// __attribute((constructor)) void _init(void)
// {
// }
// __attribute((destructor)) void _fini(void)
// {
// }

void _start(void) __attribute__((naked, aligned(4), section(".init")));
void _start(void) {
  // 通过global pointer优化内存访问，减小代码size
  // 配合 -msmall-data-limit=n 编译参数使用
  // https://blog.csdn.net/zoomdy/article/details/100703451
  __asm(".option push");
  __asm(".option norelax");
  __asm("la gp, __global_pointer$");
  __asm(".option pop");

  // 设置SP初始值
  __asm("la sp, _sp");

  // 拷贝数据到初始数据段
  uint32_t *fr = _data_lma;
  uint32_t *to = _data;
  unsigned int len = _edata - _data;
  while (len--)
    *to++ = *fr++;

  // 清除BSS段
  for (to = __bss_start; to < _end; *to++ = 0)
    ;

  // atexit(__libc_fini_array);
  // __libc_init_array();

  // __asm("auipc ra, 0");
  // __asm("addi sp, sp, -16");
  // init_stdio();
  main(); // Goto user main
          // int status = main(); // Goto user main
          // exit(status);
}

// #define CSR_MCOUNTINHIBIT      0x320
void disable_mcycle_minstret(void) __attribute__((naked, aligned(4), section(".init")));
void disable_mcycle_minstret(void)
{
	__asm("csrsi 0x320, 0x5");
}

void enable_mcycle_minstret(void) __attribute__((naked, aligned(4), section(".init")));
void enable_mcycle_minstret(void)
{
	__asm("csrci 0x320, 0x5");
}