#include"fbDraw.h"
#include"linkedList.h"
#include"structure.h"
#include<unistd.h>
#include<pthread.h>
#include <time.h>

#define BUFFER_SIZE 256
#define BALL_NUM 5
int data_available = 0;
char buffer[BUFFER_SIZE];
char cmd;
char preCMD;
static pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t ball_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t ball_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    dev_fb *fb;
    Ball *ball;
} ThreadArgs;

void *inputCMD(void *arg){
    while (1) {
        char input[BUFFER_SIZE];
        // 표준 입력에서 문자열을 읽음
        puts("입력 하세요.");
        puts("a : 공 추가");
        puts("s : 스피드 증가");
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        // 입력 데이터에 대한 잠금 획득
        pthread_mutex_lock(&buffer_mutex);
        
        // 공유 버퍼에 입력 데이터 복사
        strncpy(buffer, input, BUFFER_SIZE);
        data_available = 1;
        
        // 버퍼가 채워졌음을 출력 스레드에 알림
        pthread_cond_signal(&buffer_cond);
        
        // 잠금 해제
        pthread_mutex_unlock(&buffer_mutex);
    }
    return NULL;
}
/*
두 종류 입력
'a' = 공 추가
's' = 스피드 증가
그 외 입력 = "다시  입력하세요!"
*/
void *processCMD(void *arg){
    while (1) {
        // 잠금 획득
        pthread_mutex_lock(&buffer_mutex);
        
        // 데이터가 있을 때까지 대기
        while (!data_available) {
            pthread_cond_wait(&buffer_cond, &buffer_mutex);
        }
        
        // 버퍼의 데이터를 처리
        switch (buffer[0])
        {
        case 'a':
            puts("공을 추가 하였습니다.");
            pthread_mutex_lock(&ball_mutex);
            pthread_cond_signal(&ball_cond);
            pthread_mutex_unlock(&ball_mutex);
            break;

        case 's':
            puts("스피드를 증가하였습니다.");
            break;

        default:
            puts("다시 입력하세요!");
            break;
        }
        
        // 데이터가 처리되었음을 표시
        data_available = 0;
        
        // 잠금 해제
        pthread_mutex_unlock(&buffer_mutex);
    }
    return NULL;
}

void* ball_thread_func(void *arg) {
    pthread_mutex_lock(&ball_mutex);
    pthread_cond_wait(&ball_cond, &ball_mutex);
    pthread_mutex_unlock(&ball_mutex);

    ThreadArgs *argt = (ThreadArgs *) arg;
    
    dev_fb *fb = argt->fb;
    Ball *b1 = argt->ball;

    while (1) {
        //랜덤한 x,y : fb도 전달 받자
        fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x,b1->pos.y),255,255,255);  // erase ball at old position

        b1->pos.x += b1->speed.dx;
        b1->pos.y += b1->speed.dy;

        if (b1->pos.x <= 0 || b1->pos.x >= fb->vinfo.xres - 10) b1->speed.dx = -b1->speed.dx;
        if (b1->pos.y <= 0 || b1->pos.y >= fb->vinfo.yres - 10) b1->speed.dy = -b1->speed.dy;

        fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x,b1->pos.y),0,0,0);  // erase ball at old position
        
        usleep(10000);  // delay for smooth animation
    }
    return NULL;
}

int main() {
    pthread_t input, processor, thread[BALL_NUM];
    ThreadArgs *args[BALL_NUM];
    Ball *balls[BALL_NUM];
    dev_fb fb;
    memset(&fb, 0, sizeof(dev_fb));
    srand(time(NULL));

    fb_init(&fb);
    
    // Fill screen with a test value
    fb_fillScr(&fb, 255, 255, 255);

    pthread_create(&input, NULL, inputCMD, NULL);
    pthread_create(&processor, NULL, processCMD, NULL);

    for(int i = 0; i < 5; i++){
        //ball 객체 생성
        balls[i] = (Ball *)malloc(sizeof(Ball));
        if (balls[i] == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        balls[i]->pos.x = rand() % 1281;
        balls[i]->pos.y = rand() % 801;
        balls[i]->speed.dx = (rand() % 2 == 0) ? 2 : -2;
        balls[i]->speed.dy = (rand() % 2 == 0) ? 2 : -2;

        //스레드에 fb객체와 ball객체를 한꺼번에 전달하기 위해 객체화
        args[i] = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        if (args[i] == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        args[i]->fb = &fb;
        args[i]->ball = balls[i];

        pthread_create(&thread[i], NULL, ball_thread_func, args[i]);
    }


    for(int i = 0; i < 5; i++){
        pthread_join(thread[i], NULL);
    }
    pthread_join(input, NULL);
    pthread_join(processor, NULL);
    
    // Cleanup
    // munmap(fb.fbp, fb.screensize);
    // close(fb.fbfd);

    // node *head = (node *)malloc(sizeof(node));

    // appendNode(head, b1);

    // PrintInfo(head);

    return 0;
}
