//
// Created by loona on 8/25/21.
//

#include <memory.h>
#include <elf.h>
#include "loader.h"


void* load_elf(void *load_addr, void *mapped) {
    Elf64_Ehdr *ehdr = mapped;
    Elf64_Phdr *phdr = mapped + ehdr->e_phoff;
    void *text_segment = NULL;
    unsigned long initial_vaddr = 0;
    unsigned long brk_addr = 0;

    for(size_t i = 0; i < ehdr->e_phnum; i++, phdr++) {
        unsigned long rounded_len, k;
        void *segment;

        // Если не PT_LOAD, ничего не делаем
        if(phdr->p_type != PT_LOAD)
            continue;

        if(text_segment != 0 && ehdr->e_type == ET_DYN) {
            // Для ET_DYN phdr->p_vaddr содержит относительный виртуальный адрес,
            // для получения абсолютного виртуального адреса нужно прибавить
            // к нему базовый адрес, равный разности абсолютного и относительного
            // виртуальных адресов первого сегмента
            load_addr = text_segment + phdr->p_vaddr - initial_vaddr;
            load_addr = (void*)PAGEDOWN(load_addr);
        } else if(ehdr->e_type == ET_EXEC) {
            // Для ET_EXEC phdr->p_vaddr содержит абсолютный виртуальный адрес
            load_addr = (void*)PAGEDOWN(phdr->p_vaddr);
        }

        // Размер сегмента должен быть кратен размеру страницы
        rounded_len = phdr->p_memsz + (phdr->p_vaddr % 4096);
        rounded_len = PAGEUP(rounded_len);

        // Выделение необходимого количества памяти по заданному адресу
        segment = mmap(load_addr,
                       rounded_len,
                       PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                       -1,
                       0);

        if(ehdr->e_type == ET_EXEC)
            load_addr = (void*)phdr->p_vaddr;
        else
            load_addr = segment + (phdr->p_vaddr % 4096);
        // Копируем данные в только что выделенную область памяти
        memcpy(load_addr, mapped + phdr->p_offset, phdr->p_filesz);

        if(!text_segment) {
            text_segment = segment;
            initial_vaddr = phdr->p_vaddr;
        }

        unsigned int protflags = 0;
        if(phdr->p_flags & PF_R)
            protflags |= PROT_READ;
        if(phdr->p_flags & PF_W)
            protflags |= PROT_WRITE;
        if(phdr->p_flags & PF_X)
            protflags |= PROT_EXEC;

        mprotect(segment, rounded_len, protflags); // Задание прав
        // чтения, записи, исполнения

        k = phdr->p_vaddr + phdr->p_memsz;
        if(k > brk_addr) brk_addr = k;
    }

    if (ehdr->e_type == ET_EXEC) {
        brk(PAGEUP(brk_addr));
        // Для ET_EXEC ehdr->e_entry содержит абсолютный виртульный адрес
        load_addr = (void*)ehdr->e_entry;
    } else {
        // Для ET_DYN ehdr->e_entry содержит относительный виртуальный адрес,
        // для получения абсолютного адреса нужно прибавить к нему базовый адрес
        load_addr = (void*)ehdr + ehdr->e_entry;
    }

    return load_addr; // Возвращаем адрес точки входа в программу
}
