#include"fbDraw.hpp"
#include"linkedList.hpp"
#include"common.hpp"
#include"thread.hpp"

int main() 
{
    srand(time(NULL));

    pthread_t input, processor, thread[BALL_NUM];
    ThreadArgs *args[BALL_NUM];

    //프레임버퍼 초기화
    memset(&fb, 0, sizeof(dev_fb));

    //프래임버퍼 초기화 진행 & 실패 시 에러 내용 출력 후 프로그램 종료
    int result = fb_init(&fb);
    if (result != 0) 
    {
        fprintf(stderr, "Framebuffer initialization failed with error code %d\n", result);
        exit(1);
    }
    
    // 흰색 환명으로 칠함
    fb_fillScr(&fb, 255, 255, 255);

    //스레드 생성
    //입력 받기 위한 스레드
    pthread_create(&input, NULL, inputCMD, NULL);
    //입력을 처리하는 스레드
    pthread_create(&processor, NULL, processCMD, NULL);

    for(int i = 0; i < BALL_NUM; i++)
    {
        //ball 객체 생성
        Ball *b = (Ball *)malloc(sizeof(Ball));
        if (b == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        //ball 객체 난수값으로 초기화

        //해상도: 1280x800 기준
        //가로 난수: 0~1280
        //세로 난수: 0~800
        b->pos.x = rand() % fb.vinfo.xres + 1;
        b->pos.y = rand() % fb.vinfo.yres + 1;
        b->speed.dx = (rand() % 2 == 0) ? 2 : -2;
        b->speed.dy = (rand() % 2 == 0) ? 2 : -2;

        //스레드에 framebuffer객체와 ball객체를 한꺼번에 전달하기 위해 객체화
        args[i] = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        if (args[i] == NULL) 
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        //인자들의 묶음 객체(args) 초기화
        args[i]->fb = &fb;
        args[i]->ball = b;

        appendNode(head, b);

        pthread_create(&thread[i], NULL, ball_thread_func, args[i]);
    }
    
    //스레드들 join 진행 
    pthread_join(input, NULL);
    pthread_join(processor, NULL);

    for(int i = 0; i < BALL_NUM; i++)
        pthread_join(thread[i], NULL);

    return 0;
}