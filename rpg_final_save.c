#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));
    char buf[256];

    printf("=== БИТВА С ДРАКОНОМ (С сохранением) ===\n");
    system("cat wins.txt");
    printf("Введи свое имя:\n");
    int name = input_str;
    int hp = 100;
    int enemy_hp = 100;
    int game_over = 0;
    while (game_over == 0) {
    printf("Твое HP:\n");
    printf("%d\n", hp);
    printf("HP Дракона:\n");
    printf("%d\n", enemy_hp);
    printf("1. Атака\n");
    printf("2. Зелье (+30)\n");
    printf("3. Выход\n");
    int action = input();
    if (action == 1) {
    int dmg = 10 + rand() % (25 - 10 + 1);
    printf("Ты нанес урон:\n");
    printf("%d\n", dmg);
    enemy_hp = enemy_hp - dmg;
    }
    if (action == 2) {
    hp = hp + 30;
    printf("Лечение!\n");
    }
    if (action == 3) {
    int game_over = 1;
    }
    if (enemy_hp > 0) {
    int e_dmg = 5 + rand() % (15 - 5 + 1);
    printf("Дракон бьет на:\n");
    printf("%d\n", e_dmg);
    hp = hp - e_dmg;
    }
    if (hp <= 0) {
    int game_over = 1;
    printf("Ты проиграл...\n");
    }
    if (enemy_hp <= 0) {
    int game_over = 1;
    printf("Ты победил!\n");
    system("echo 1 >> wins.txt");
    }
    }
    free(pool);
    return 0;
}
