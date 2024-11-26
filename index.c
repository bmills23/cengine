#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for holding key-value pairs
typedef struct {
    char *key;
    char *value;
} Data;

// Function prototypes
void render_template(const char *template, Data *data, int data_count, char *output);

int main() {
    // Sample template string
    const char *template = 
        "<html>\n"
        "<head><title>{{title}}</title></head>\n"
        "<body>\n"
        "    <h1>{{heading}}</h1>\n"
        "    <p>{{content}}</p>\n"
        "</body>\n"
        "</html>";

    // Data to replace placeholders
    Data data[] = {
        {"title", "Template Challenge"},
        {"heading", "Welcome to the Challenge!"},
        {"content", "Your task is to complete the templating engine logic."}
    };

    int data_count = sizeof(data) / sizeof(data[0]);

    // Output buffer for the rendered template
    char output[1024] = {0};

    // Render the template
    render_template(template, data, data_count, output);

    // Print the rendered template
    printf("%s\n", output);

    return 0;
}

// Your task: Implement the render_template function
void render_template(const char *template, Data *data, int data_count, char *output) {
    // This is your challenge!
    // - Parse the template string
    // - Detect placeholders like {{key}}
    // - Replace placeholders with values from the data array
    // - Copy everything else directly into the output buffer

    // Declare pointer for template to preserve read-only 
    char *ptr = template;

    while(*ptr)
    {   
        if (*ptr == '{' && *(ptr + 1) == '{')
        {
            const char *start = ptr + 2;
            const char *end = strstr(start, '}}');
        }
    }

}