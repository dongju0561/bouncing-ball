#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

#define NUM_BALLS 5
#define BALL_SIZE 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000

typedef struct {
    int x, y;
    int dx, dy;
    int color;
} Ball;

typedef struct {
    int width;
    int height;
    int line_length;
    int bpp;
    char *fbp;
} FrameBuffer;

FrameBuffer fb;
pthread_mutex_t fb_mutex;

void draw_ball(Ball *ball, int color) {
    pthread_mutex_lock(&fb_mutex);
    for (int i = 0; i < BALL_SIZE; i++) {
        for (int j = 0; j < BALL_SIZE; j++) {
            int x = ball->x + i;
            int y = ball->y + j;
            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                long location = (x + y * fb.width) * (fb.bpp / 8);
                *((unsigned int *)(fb.fbp + location)) = color;
            }
        }
    }
    pthread_mutex_unlock(&fb_mutex);
}

void* ball_thread_func(void *arg) {
    Ball *ball = (Ball *)arg;
    while (1) {
        draw_ball(ball, COLOR_BLACK);  // erase ball at old position

        ball->x += ball->dx;
        ball->y += ball->dy;

        if (ball->x <= 0 || ball->x >= SCREEN_WIDTH - BALL_SIZE) ball->dx = -ball->dx;
        if (ball->y <= 0 || ball->y >= SCREEN_HEIGHT - BALL_SIZE) ball->dy = -ball->dy;

        draw_ball(ball, ball->color);  // draw ball at new position

        usleep(30000);  // delay for smooth animation
    }
    return NULL;
}

void init_framebuffer(FrameBuffer *fb) {
    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
        perror("Error opening framebuffer device");
        exit(1);
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    fb->width = vinfo.xres;
    fb->height = vinfo.yres;
    fb->line_length = finfo.line_length;
    fb->bpp = vinfo.bits_per_pixel;

    long screensize = vinfo.yres_virtual * finfo.line_length;

    fb->fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb->fbp == MAP_FAILED) {
        perror("Error mapping framebuffer device to memory");
        exit(4);
    }
}

int main() {
    Ball balls[NUM_BALLS];
    pthread_t threads[NUM_BALLS];
    srand(time(NULL));

    // Initialize framebuffer
    init_framebuffer(&fb);
    pthread_mutex_init(&fb_mutex, NULL);

    // Initialize balls
    for (int i = 0; i < NUM_BALLS; i++) {
        balls[i].x = rand() % (SCREEN_WIDTH - BALL_SIZE);
        balls[i].y = rand() % (SCREEN_HEIGHT - BALL_SIZE);
        balls[i].dx = (rand() % 2 == 0) ? 2 : -2;
        balls[i].dy = (rand() % 2 == 0) ? 2 : -2;
        balls[i].color = COLOR_WHITE;
        
        pthread_create(&threads[i], NULL, ball_thread_func, &balls[i]);
    }

    // Join threads (in practice, you'd likely have a more graceful exit condition)
    for (int i = 0; i < NUM_BALLS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&fb_mutex);
    munmap(fb.fbp, fb.height * fb.line_length);

    return 0;
}
