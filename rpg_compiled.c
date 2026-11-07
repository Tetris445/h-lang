#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));
    char buf[256];

    printf("=== БИТВА С ДРАКОНОМ (Скомпилировано!) ===\n");
    int hp = 100;
    int enemy_hp = 100;
    int game_over = 0;
    printf("Твое HP:\n");
    printf("hp\n");
    printf("HP Дракона:\n");
    printf("enemy_hp\n");
    printf("1. Атака\n");
    printf("2. Зелье (+30)\n");
    printf("3. Выход\n");
    int action = input();
    if (action == "1") {
    int dmg = random 10 25;
    printf("Ты нанес урон:\n");
    printf("dmg\n");
    enemy_hp = sub enemy_hp dmg;
    } else {
    if (action == "2") {
    hp = add hp 30;
    printf("Лечение!\n");
    } else {
    int game_over = 1;
    }
    }
    if (enemy_hp > 0) {
    int e_dmg = random 5 15;
    printf("Дракон бьет на:\n");
    printf("e_dmg\n");
    hp = sub hp e_dmg;
    }
    if (hp <= 0) {
    int game_over = 1;
    printf("Ты проиграл...\n");
    }
    if (enemy_hp <= 0) {
    int game_over = 1;
    printf("Ты победил!\n");
    }
    }
    free(pool);
    return 0;
}
