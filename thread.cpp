#include "thread.hpp"

int data_available = 0;
char buffer[BUFFER_SIZE];

// 공 속도 변수
int speed = 10000;

pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ball_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ball_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t input, processor, thread[BALL_NUM];
ThreadArgs *args[BALL_NUM];

int thread_index = 0;

//사용자 입력 받는 스레드 함수
void *inputCMD(void *arg)
{
    while (1) 
    {
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
//입력 받은 값을 처리하는 스레드 함수 
void *processCMD(void *arg)
{
    Ball *b;
    ThreadArgs *args;
    while (1) 
    {
        // 잠금 획득
        pthread_mutex_lock(&buffer_mutex);
        
        // 데이터가 있을 때까지 대기
        while (!data_available) 
        {
            pthread_cond_wait(&buffer_cond, &buffer_mutex);
        }
        
        // 버퍼의 데이터를 처리
        /*
        두 종류 입력
        'a' = 공 추가
        's' = 스피드 증가
        'd' = 스피드 감소
        'x' = 프로그램 종료, 메모리 해제
        그 외 입력 = 동작 없음
        */
        switch (buffer[0]) 
        {
        case 'a':
            // ball 객체 생성
            b = (Ball *)malloc(sizeof(Ball));
            if (b == NULL) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
            // ball 객체 난수값으로 초기화

            //해상도: 1280x800 기준
            //가로 난수: 0~1280
            //세로 난수: 0~800
            b->pos.x = rand() % fb.vinfo.xres + 1;
            b->pos.y = rand() % fb.vinfo.yres + 1;
            b->speed.dx = (rand() % 2 == 0) ? 2 : -2;
            b->speed.dy = (rand() % 2 == 0) ? 2 : -2;

            //스레드에 framebuffer객체와 ball객체를 한꺼번에 전달하기 위해 객체화
            args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
            if (args == NULL) 
            {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
            // //인자들의 묶음 객체(args) 초기화
            args->fb = &fb;
            args->ball = b;

            append_node(head, b);

            pthread_create(&thread[thread_index++], NULL, ball_thread_func, args);
            //스레드를 추가할때 마다 인덱스 증가

            break;
        case 'z':
            //스레드 삭제
            thread_index = thread_index - 1;
            pthread_cancel(thread[thread_index]);
            
            //메모리 해제
            delete_last_node(head);

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
            exit(1);
            close_list(head);
            // 프래임버퍼 메모리 해제
            fb_close(&fb);
            
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

void cleanup(void *arg) {
    ThreadArgs *argt = (ThreadArgs *) arg;
    dev_fb *fb = argt->fb;
    Ball *b1 = argt->ball;

    fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x, b1->pos.y), 255, 255, 255);
}

//프래임버퍼 데이터와 ball 객체를 전달 받아 프래임버퍼에 ball의 데이터에 따라 화면에 출력하는 스레드 함수

void* ball_thread_func(void *arg) 
{
    ThreadArgs *argt = (ThreadArgs *) arg;
    
    dev_fb *fb = argt->fb;
    Ball *b1 = argt->ball;

    pthread_cleanup_push(cleanup, (void *)argt);

    while (1) 
    {
        //전달 받은 ball 객체의 좌표 데이터에 따라 흰색 원을 화면에 출력(원 지움)
        fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x, b1->pos.y), 255, 255, 255);

        //현재위치(pos.x, pos.y)에서 b1->speed.dx, b1->speed.dy만큼 이동
        b1->pos.x += b1->speed.dx;
        b1->pos.y += b1->speed.dy;

        //ball이 벽에 부딪혔을때
        if (b1->pos.x <= 0 || b1->pos.x >= fb->vinfo.xres - 10) b1->speed.dx = -b1->speed.dx;
        if (b1->pos.y <= 0 || b1->pos.y >= fb->vinfo.yres - 10) b1->speed.dy = -b1->speed.dy;

        //전달 받은 ball 객체의 좌표 데이터에 따라 검은색 원을 화면에 출력(원 칠함)
        fb_drawFilledCircle(fb, fb_toPixel(b1->pos.x, b1->pos.y), 0, 0, 0);
        
        usleep(speed);  // delay for smooth animation
    }
    return NULL;

    pthread_cleanup_pop(0);
}
