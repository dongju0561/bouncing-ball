#include <stdio.h>
#include <string.h>

#define WIDTH 100
#define HEIGHT 100

void draw_filled_circle(unsigned char framebuffer[HEIGHT][WIDTH], int center_x, int center_y, int radius, unsigned char color) {
    int x0 = center_x;
    int y0 = center_y;
    int r = radius;

    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        // Draw horizontal lines to fill the circle
        for (int dx = -x; dx <= x; dx++) {
            framebuffer[y0 + y][x0 + dx] = color;
            framebuffer[y0 - y][x0 + dx] = color;
            framebuffer[y0 + x][x0 + dx] = color;
            framebuffer[y0 - x][x0 + dx] = color;
        }

        y += 1;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) {
            x -= 1;
            err += 1 - 2 * x;
        }
    }
}

void print_framebuffer(unsigned char framebuffer[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (framebuffer[y][x]) {
                putchar('#');
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
}

int main() {
    unsigned char framebuffer[HEIGHT][WIDTH];
    memset(framebuffer, 0, sizeof(framebuffer)); // 프레임버퍼를 0으로 초기화

    int center_x = 50;
    int center_y = 50;
    int radius = 20;
    unsigned char color = 1; // 색은 1로 설정 (예: 흰색)

    draw_filled_circle(framebuffer, center_x, center_y, radius, color);

    print_framebuffer(framebuffer);

    return 0;
}
