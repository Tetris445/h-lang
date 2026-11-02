#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char response[1024] = {0};

    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8081);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("C-сервер запущен на порту 8081...\n");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        read(new_socket, buffer, 1024);
        
        // Обработка команды
        if (strncmp(buffer, "add ", 4) == 0) {
            int a, b;
            if (sscanf(buffer + 4, "%d %d", &a, &b) == 2) sprintf(response, "%d\n", a + b);
        } else if (strncmp(buffer, "mul add ", 8) == 0) {
            int a, b, c;
            if (sscanf(buffer + 8, "%d %d %d", &a, &b, &c) == 3) sprintf(response, "%d\n", a * (b + c));
        } else {
            sprintf(response, "Unknown command\n");
        }
        
        send(new_socket, response, strlen(response), 0);
        close(new_socket);
        memset(buffer, 0, sizeof(buffer));
        memset(response, 0, sizeof(response));
    }
    return 0;
}
