#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "hex_to_text.h"

#define PORT 8080
#define BUFFER_SIZE 4096

void send_response(int client_socket, const char *header, const char *content) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "%s\r\nContent-Length: %ld\r\n\r\n%s",
             header, strlen(content), content);
    write(client_socket, response, strlen(response));
}

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        perror("Lỗi đọc request");
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';

    if (strncmp(buffer, "GET / ", 6) == 0) {
        char html_page[] =
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html lang='vi'>"
            "<head><meta charset='UTF-8'><title>Hex to Text</title></head>"
            "<body>"
            "<h2>Hex to Text Converter</h2>"
            "<form method='POST' action='/convert'>"
            "<label>Hex:</label>"
            "<input type='text' name='hex'>"
            "<button type='submit'>Convert</button>"
            "</form>"
            "</body></html>";
        
        write(client_socket, html_page, strlen(html_page));
    } 
    else if (strncmp(buffer, "POST /convert", 13) == 0) {
        char *hex_start = strstr(buffer, "hex=");
        if (!hex_start) {
            send_response(client_socket, "HTTP/1.1 400 Bad Request", "Lỗi dữ liệu");
            close(client_socket);
            return;
        }

        hex_start += 4;
        char hex_string[256];
        sscanf(hex_start, "%255s", hex_string);

        char text_output[128];
        hex_to_text(hex_string, text_output);

        char response_html[BUFFER_SIZE];
        snprintf(response_html, sizeof(response_html),
                 "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                 "<html><body>"
                 "<h2>Results</h2>"
                 "<p>Hex: %s</p>"
                 "<p>Text: %s</p>"
                 "<a href='/'>Return</a>"
                 "</body></html>",
                 hex_string, text_output);

        write(client_socket, response_html, strlen(response_html));
    } 
    else {
        send_response(client_socket, "HTTP/1.1 404 Not Found", "Lỗi 404");
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen lỗi");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server đang chạy trên http://localhost:%d\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept lỗi");
            continue;
        }

        handle_request(client_socket);
    }

    close(server_fd);
    return 0;
}

