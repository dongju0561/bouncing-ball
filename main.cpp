#include"fbDraw.hpp"
#include"linkedList.hpp"
#include"common.hpp"
#include"thread.hpp"

extern pthread_t input, processor, thread[BALL_NUM];

int main() 
{
    //난수를 위한 시드 생성
    srand(time(NULL));

    

    //프레임버퍼 초기화
    memset(&fb, 0, sizeof(dev_fb));

    //프래임버퍼 초기화 진행 & 실패 시 에러 내용 출력 후 프로그램 종료
    int result = fb_init(&fb);
    if (result != 0) 
    {
        fprintf(stderr, "Framebuffer initialization failed with error code %d\n", result);
        exit(1);
    }
    
    // 흰색 화면으로 칠함
    fb_fillScr(&fb, 255, 255, 255);

    //스레드 생성
    //입력 받기 위한 스레드
    pthread_create(&input, NULL, inputCMD, NULL);
    //입력을 처리하는 스레드
    pthread_create(&processor, NULL, processCMD, NULL);

    
    //스레드들 join 진행 
    pthread_join(input, NULL);
    pthread_join(processor, NULL);

    for(int i = 0; i < BALL_NUM; i++)
        pthread_join(thread[i], NULL);

    return 0;
}