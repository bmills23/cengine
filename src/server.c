#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>

// Includes read_file & serve_file
#include "common.h"

int MAX_FILENAME_LENGTH = 100;

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    
    // Function to initialize Winsock on Windows
    void initialize_winsock() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            fprintf(stderr, "Failed to initialize Winsock.\n");
            exit(EXIT_FAILURE);
        }
    }

    #include <windows.h>

    #define CLOSE_SOCKET closesocket
    typedef int ssize_t;
#else
    // Includes for Unix-like systems
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define CLOSE_SOCKET close
#endif

#define PORT 8080
#define BUFFER_SIZE 1024

void error_exit(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
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

void handle_client_request(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) {
        perror("recv failed");
        return;
    }

    buffer[bytes_read] = '\0';
    printf("Request received:\n%s\n", buffer);

    // Extract the requested route
    char method[16], route[256];
    sscanf(buffer, "%s %s", method, route);

    // Directory Read for Page rendering
    #if defined(_WIN32) || defined(_WIN64)
        const char *directory_path = "../pages"; // page directory
        WIN32_FIND_DATA find_file_data;
        HANDLE hFind;

        char search_path[MAX_PATH];
        snprintf(search_path, MAX_PATH, "%s\\*", directory_path); // Append wildcard

        hFind = FindFirstFile(search_path, &find_file_data);

        if (hFind == INVALID_HANDLE_VALUE) {
            printf("Error opening directory\n");
            return;
        }

        printf("Files in directory '%s':\n", directory_path);

        do {
            printf("%s\n", find_file_data.cFileName); // Print file name
        } while (FindNextFile(hFind, &find_file_data) != 0);

        FindClose(hFind); // Close handle
    # else
        const char *directory_path = "../pages"; // pages directory
        struct dirent *entry;          
        DIR *directory = opendir(directory_path); 

        if (directory == NULL) {
            perror("Unable to open directory");
            return;
        }

        printf("Files in directory '%s':\n", directory_path);

        // Amount of files in the pages directory
        int file_amount;

        while ((entry = readdir(directory)) != NULL) {
            file_amount++;
        }

        // Allocate array for file names
        char** files = malloc(file_amount * sizeof(char*));

        if (files == NULL) {
            perror("Memory allocation failed");
            return; // Handle memory allocation failure
        }

        // Example: Allocate memory for each string
        for (int i = 0; i < file_amount; i++) {
            files[i] = malloc(MAX_FILENAME_LENGTH * sizeof(char));
            if (files[i] == NULL) {
                perror("Memory allocation failed for string");
                return; // Handle memory allocation failure
            }
        }

        // // Free memory after usage
        // for (int i = 0; i < file_amount; i++) {
        //     free(files[i]);
        // }

        // free(files);

        int i = 0;

        // Not working
        while ((entry = readdir(directory)) != NULL) {
            files[i] = entry->d_name;
            printf("File Name: %s\n", files[i]);
            i++;
        }

        closedir(directory); // Close directory
    # endif

    // Check the route and serve the appropriate file
    if (strcmp(route, "/") == 0) {
        serve_file(client_fd, "../pages/index.html", "text/html");
    } else if (strcmp(route, "/about") == 0) {
        serve_file(client_fd, "../pages/about.html", "text/html");
    } else {
        const char* not_found_response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "404 Not Found";
        send(client_fd, not_found_response, strlen(not_found_response), 0);
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    #if defined(_WIN32) || defined(_WIN64)
        initialize_winsock();
    #endif

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        error_exit("Socket creation failed");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_exit("Bind failed");
    }

    if (listen(server_fd, 5) == -1) {
        error_exit("Listen failed");
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Connection established with client: %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        handle_client_request(client_fd);
        CLOSE_SOCKET(client_fd);
    }

    CLOSE_SOCKET(server_fd);

    #if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
    #endif

    printf("Server shut down.\n");
    return 0;
}
