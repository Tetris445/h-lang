#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: h2c <file.h>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("File not found: %s\n", argv[1]);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Убираем символ новой строки
        line[strcspn(line, "\n")] = 0;

        // Пропускаем пустые строки и комментарии
        if (line[0] == '\0' || line[0] == '#') continue;

        // Команда say
        if (strncmp(line, "say ", 4) == 0) {
            printf("%s\n", line + 4);
        }
    }

    fclose(file);
    return 0;
}
