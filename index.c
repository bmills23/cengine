#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for holding key-value pairs
typedef struct {
    const char *key;
    const char *value;
} Data;

// Function prototypes
void render_template(const char *template, Data *data, int data_count, char **output);

int main() {
    // Sample template string
    FILE *file = fopen("index.html", "rb");
    if (!file)
    {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END); // seek to end of file
    size_t file_size = ftell(file); // get current file pointer
    rewind(file); // seek back to beginning of file
    
    if (file_size < 0)
    {
        perror("Failed to get file size");
        fclose(file);
        return 1;
    }

    printf("Size = %u\n", file_size);

    // Allocate template for file content
    char *template = (char*)malloc(file_size + 1);

    if (!template)
    {
        perror("Failed to process template");
        fclose(file);
        return 1;
    }

    size_t read_size = fread(template, 1, file_size, file);
    
    printf("read_size = %u\n", read_size);

    if (read_size != file_size)
    {
        perror("Failed to read the file");
        free(template);
        fclose(file);
        return 1;
    }

    // Data to replace placeholders
    Data data[] = {
        {"title", "Template Challenge"},
        {"heading", "Welcome to the Challenge!"},
        {"content", "My name is Bryan."}
    };

    int data_count = sizeof(data) / sizeof(data[0]);
    char *output = NULL;

    // Render the template
    render_template(template, data, data_count, &output);

    // Free template once done with render_template
    free(template);

    if (output) {
        // Print the rendered template
        printf("%s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Failed to render template\n");
    }

    return 0;
}

void render_template(const char *template, Data *data, int data_count, char **output) {
    size_t buffer_size = 1024; // Initial buffer size
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        fprintf(stderr, "Memory allocation failed for template buffer\n");
        return;
    }

    buffer[0] = '\0'; // Initialize buffer
    const char *ptr = template;

    while (*ptr) {
        if (*ptr == '{' && *(ptr + 1) == '{') {
            // Find the closing braces
            const char *start = ptr + 2;
            const char *end = strstr(start, "}}");
            if (end) {
                // Extract the key
                size_t key_size = end - start;
                char key[key_size + 1];
                strncpy(key, start, key_size);
                key[key_size] = '\0';

                // Find the matching value
                const char *replacement = NULL;
                for (int i = 0; i < data_count; i++) {
                    if (strcmp(key, data[i].key) == 0) {
                        replacement = data[i].value;
                        break;
                    }
                }

                // Add the replacement to the buffer
                size_t replacement_size = replacement ? strlen(replacement) : 0;
                size_t new_size = strlen(buffer) + replacement_size + 1;

                if (new_size > buffer_size) {
                    buffer_size = new_size * 2; // Double the buffer size
                    char *new_buffer = realloc(buffer, buffer_size);
                    if (!new_buffer) {
                        fprintf(stderr, "Memory reallocation failed\n");
                        free(buffer);
                        return;
                    }
                    buffer = new_buffer;
                }

                if (replacement) {
                    strcat(buffer, replacement);
                }

                ptr = end + 2; // Move past }}
                continue;
            }
        }

        // Add the current character to the buffer
        size_t new_size = strlen(buffer) + 2;
        if (new_size > buffer_size) {
            buffer_size = new_size * 2; // Double the buffer size
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                fprintf(stderr, "Memory reallocation failed\n");
                free(buffer);
                return;
            }
            buffer = new_buffer;
        }

        strncat(buffer, ptr, 1);
        ptr++;
    }

    *output = buffer;
}