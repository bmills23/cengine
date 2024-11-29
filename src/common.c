#include <stdio.h>
#include <stdlib.h>

#include "common.h"

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Standard html read
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* content = malloc(file_size + 1);
    if (!content) {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);
    return content;
}