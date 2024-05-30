#include"fbDraw.h"
#include"linkedList.h"
#include<unistd.h>

int main() {
    dev_fb fb;
    memset(&fb, 0, sizeof(dev_fb));

    // node *head = (node *)malloc(sizeof(node));

    // ball *b1 = (ball *)malloc(sizeof(ball));

    // b1->pos = fb_toPixel(640,400);
    // Speed s1;
    // s1.dx = 1;
    // s1.dy = -1;
    // s1.speed = 10;

    // appendNode(head, b1);

    // PrintInfo(head);

    fb_init(&fb);
    
    // Fill screen with a test value
    fb_fillScr(&fb, 255, 255, 255);

    int init_x = 640;
    int init_y = 400;

    for(int i = 0; i < 100; i++){
        fb_drawFilledCircle(&fb, fb_toPixel(init_x, init_y),0,0,0);
        init_x += 50;
        init_y += 50;
        sleep(1);
    }
    
    // Cleanup
    munmap(fb.fbp, fb.screensize);
    close(fb.fbfd);

    return 0;
}
