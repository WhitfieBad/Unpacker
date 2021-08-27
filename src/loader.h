//
// Created by loona on 8/25/21.
//

#ifndef UNPACKER_LOADER_H
#define UNPACKER_LOADER_H

#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_ALIGN  (1<<12)
#define ROUNDUP(x,b) (((unsigned long)x+b-1)&(~(b-1)))
#define ROUNDDOWN(x,b) ((unsigned long)x&(~(b-1)))
#define PAGEUP(x) ROUNDUP(x, PAGE_ALIGN)
#define PAGEDOWN(x) ROUNDDOWN(x, PAGE_ALIGN)

void* elf_load_addr(void *rsp, void *mapped, size_t mapped_size);
void* load_elf(void *load_addr, void *mapped);

#endif //UNPACKER_LOADER_H
