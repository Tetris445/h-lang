#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: sys_net <sys|net> <command>\n");
        return 1;
    }

    char *mode = argv[1];
    char *cmd = argv[2];

    if (strcmp(mode, "sys") == 0) {
        // Выполняем системную команду
        system(cmd);
    } 
    else if (strcmp(mode, "net") == 0) {
        // Сетевой запрос через curl (если установлен)
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "curl -s %s", cmd);
        system(buffer);
    } 
    else {
        printf("Unknown mode: %s\n", mode);
    }

    return 0;
}
