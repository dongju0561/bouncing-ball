#include"fbDraw.h"
#include"linkedList.h"
#include"structure.h"
#include"thread.h"
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>
#include <time.h>

int main() {
    srand(time(NULL));
    pthread_t input, processor, thread[BALL_NUM];
    ThreadArgs *args[BALL_NUM];
    //프레임버퍼 초기화
    memset(&fb, 0, sizeof(dev_fb));

    int result = fb_init(&fb);
    if (result != 0) {
        fprintf(stderr, "Framebuffer initialization failed with error code %d\n", result);
        // 오류에 따른 추가적인 처리를 할 수 있습니다.
    }
    
    // 흰색 환명으로 칠함
    fb_fillScr(&fb, 255, 255, 255);

    //스레드 생성
    pthread_create(&input, NULL, inputCMD, NULL);
    pthread_create(&processor, NULL, processCMD, NULL);

    for(int i = 0; i < BALL_NUM; i++){
        //ball 객체 생성
        Ball *b = (Ball *)malloc(sizeof(Ball));
        if (b == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        b->pos.x = rand() % 1281;
        b->pos.y = rand() % 801;
        b->speed.dx = (rand() % 2 == 0) ? 2 : -2;
        b->speed.dy = (rand() % 2 == 0) ? 2 : -2;

        //스레드에 framebuffer객체와 ball객체를 한꺼번에 전달하기 위해 객체화
        args[i] = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        if (args[i] == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        args[i]->fb = &fb;
        args[i]->ball = b;

        appendNode(head, b);

        pthread_create(&thread[i], NULL, ball_thread_func, args[i]);
    }
    
    //스레드들 join 진행 
    pthread_join(input, NULL);
    pthread_join(processor, NULL);

    for(int i = 0; i < BALL_NUM; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}
