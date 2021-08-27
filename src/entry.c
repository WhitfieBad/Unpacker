//
// Created by loona on 8/22/21.
//
#include <link.h>
#include "entry.h"

extern uint8_t _binary_text_zip_start[];
extern uint8_t _binary_text_zip_end[];

int main() {

    void *extracted_exutable = NULL;
    size_t size_extracted_exutable = 0;
    void *data = _binary_text_zip_start;
    size_t size = (size_t)(_binary_text_zip_end - _binary_text_zip_start);

    struct zip_t *ziped_text = zip_stream_open(data, size, 0, 'r');
    zip_entry_open(ziped_text, "a.out");
    zip_entry_read(ziped_text, (void **)&extracted_exutable, &size_extracted_exutable);
    zip_entry_close(ziped_text);
    zip_stream_close(ziped_text);

    void* d = load_elf(NULL, data);
    free(extracted_exutable);
    JMP(d);
}