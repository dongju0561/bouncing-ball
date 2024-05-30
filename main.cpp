#include"fbDraw.h"
#include"linkedList.h"
#include<unistd.h>
#include<pthread.h>

char cmd;
char preCMD;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *inputCMD(void *arg){
    // while (1)
    // {
        pthread_mutex_lock(&mutex);
        scanf("%c",&cmd);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
    // }
    return NULL;
}

void *detechChange(void *arg){
    // while (1)
    // {
        pthread_cond_wait(&cond, &mutex);
        printf("%d\n",preCMD == cmd);
        // pthread_mutex_lock(&mutex);
        // if(preCMD != cmd){
        //     printf("%c\n",preCMD);
        //     preCMD = cmd;
        //     printf("%c\n",cmd);
        // }
        // printf("%c\n",cmd);
        // pthread_mutex_unlock(&mutex);
    // }
    return NULL;
}

int main() {
    pthread_t input, checker;

    pthread_create(&input, NULL, inputCMD, NULL);
    pthread_create(&checker, NULL, detechChange, NULL);

    pthread_join(input, NULL);
    pthread_join(checker, NULL);
    // dev_fb fb;
    // memset(&fb, 0, sizeof(dev_fb));

    // node *head = (node *)malloc(sizeof(node));

    // ball *b1 = (ball *)malloc(sizeof(ball));

    // b1->pos = fb_toPixel(640,400);
    // Speed s1;
    // s1.dx = 1;
    // s1.dy = -1;
    // s1.speed = 10;

    // appendNode(head, b1);

    // PrintInfo(head);

    // fb_init(&fb);
    
    // // Fill screen with a test value
    // fb_fillScr(&fb, 255, 255, 255);

    // int init_x = 640;
    // int init_y = 400;

    // for(int i = 0; i < 100; i++){
    //     fb_drawFilledCircle(&fb, fb_toPixel(init_x, init_y),0,0,0);
    //     init_x += 50;
    //     init_y += 50;
    //     sleep(1);
    // }
    
    // // Cleanup
    // munmap(fb.fbp, fb.screensize);
    // close(fb.fbfd);

    return 0;
}
