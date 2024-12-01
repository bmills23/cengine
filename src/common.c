#include <stdio.h>
#include <stdlib.h>

#include "common.h"

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

void serve_file(int client_fd, const char* filename, const char* content_type) {
    char* file_content = read_file(filename);
    if (!file_content) {
        const char* not_found_response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "404 Not Found";
        send(client_fd, not_found_response, strlen(not_found_response), 0);
        return;
    }

    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %u\r\n\r\n%s",
             content_type, (unsigned int)strlen(file_content), file_content);
    send(client_fd, response, strlen(response), 0);

    free(file_content);
}