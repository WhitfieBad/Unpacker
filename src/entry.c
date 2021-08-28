//
// Created by loona on 8/22/21.
//
#include <link.h>
#include "entry.h"

extern uint8_t _binary_a_out_start[];
extern uint8_t _binary_a_out_end[];

int main() {
    void *data = _binary_a_out_start;
    size_t size = (size_t)(_binary_a_out_end - _binary_a_out_start);
    void* d = load_elf(NULL, data);
    JMP(d);
}