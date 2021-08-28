//
// Created by loona on 8/22/21.
//

#ifndef UNPACKERARM_ENTRY_H
#define UNPACKERARM_ENTRY_H
#include "lib.h"
#include "loader.h"
#define JMP(addr) __asm__ __volatile__ ("jmp *%0"::"r"(addr))

#endif //UNPACKERARM_ENTRY_H
