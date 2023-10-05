#ifndef SYSCALL_H_
#define SYSCALL_H_

#include <stdint.h> /* uint32_t */

extern int FileDescriptorIndex;

extern void initFDT();                      
extern void closeFDT();
extern void SyscallExe(uint32_t SID); 

#endif
