#include"fbDraw.h"

int main() {
    dev_fb fb;
    memset(&fb, 0, sizeof(dev_fb));

    fb_init(&fb);
    
    // Fill screen with a test value
    fb_fillScr(&fb, 0, 0, 0);

    fb_drawFilledCircle(&fb, fb_toPixel(640, 400),255,255,255);

    // Cleanup
    munmap(fb.fbp, fb.screensize);
    close(fb.fbfd);
    return 0;
}
