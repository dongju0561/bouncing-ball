#include "thread.h"

int data_available = 0;
char buffer[BUFFER_SIZE];

// 공 속도 변수
int speed = 10000;

pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ball_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ball_mutex = PTHREAD_MUTEX_INITIALIZER;

void *inputCMD(void *arg){
    while (1) {
        char input[BUFFER_SIZE];
        // 표준 입력에서 문자열을 읽음
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

void *processCMD(void *arg){
    while (1) {
        // 잠금 획득
        pthread_mutex_lock(&buffer_mutex);
        
        // 데이터가 있을 때까지 대기
        while (!data_available) {
            pthread_cond_wait(&buffer_cond, &buffer_mutex);
        }
        
        // 버퍼의 데이터를 처리
        switch (buffer[0]) {
        /*
        두 종류 입력
        'a' = 공 추가
        's' = 스피드 증가
        'd' = 스피드 감소
        그 외 입력 = 동작 없음
        */
        case 'a':
            pthread_mutex_lock(&ball_mutex);
            pthread_cond_signal(&ball_cond);
            pthread_mutex_unlock(&ball_mutex);
            break;
        case 's':
            speed -= 1000;
            break;
        case 'd':
            speed += 1000;
            break;
        case 'p':
            PrintInfo(head);
            break;
        case 'x':
            // 프로그램 종료
            close_list(head);
            // 프래임버퍼 메모리 정리
            munmap(fb.fbp, fb.screensize);
            close(fb.fbfd);
            exit(1);
            break;
        default:
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
        // 랜덤한 x, y : fb도 전달 받자
        fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x, b1->pos.y), 255, 255, 255);  // erase ball at old position

        b1->pos.x += b1->speed.dx;
        b1->pos.y += b1->speed.dy;

        if (b1->pos.x <= 0 || b1->pos.x >= fb->vinfo.xres - 10) b1->speed.dx = -b1->speed.dx;
        if (b1->pos.y <= 0 || b1->pos.y >= fb->vinfo.yres - 10) b1->speed.dy = -b1->speed.dy;

        fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x, b1->pos.y), 0, 0, 0);  // erase ball at old position
        
        usleep(speed);  // delay for smooth animation
    }
    return NULL;
}
