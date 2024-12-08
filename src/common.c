#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

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

 // Remove .html extension
void remove_html_extension(char *filename) 
{
    char *extension = strstr(filename, ".html"); // Find ".html" in the string
    if (extension != NULL) 
    {
        *extension = '\0'; // Truncate the string at the start of ".html"
    }
}

