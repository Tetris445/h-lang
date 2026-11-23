// gfx.c — Драйвер framebuffer для QEMU virt (PL111)
#define FB_BASE  0x3C000000 // Базовый адрес памяти для экрана
#define FB_WIDTH  640
#define FB_HEIGHT 480
#define FB_BPP    4 // 4 байта на пиксель (32 бита)

volatile unsigned int *framebuffer = (unsigned int*)FB_BASE;

// Установить пиксель (x, y) цветом color (0x00RRGGBB)
void put_pixel(int x, int y, unsigned int color) {
    if (x >= 0 && x < FB_WIDTH && y >= 0 && y < FB_HEIGHT) {
        framebuffer[y * FB_WIDTH + x] = color;
    }
}

// Залить весь экран цветом
void clear_screen(unsigned int color) {
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        framebuffer[i] = color;
    }
}

// Нарисовать прямоугольник
void draw_rect(int x, int y, int w, int h, unsigned int color) {
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            put_pixel(j, i, color);
        }
    }
}

// Нарисовать простую букву (заглушка: просто квадрат)
void draw_char(int x, int y, unsigned int color) {
    draw_rect(x, y, 8, 8, color); // Просто квадрат 8x8
}
