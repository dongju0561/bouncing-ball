#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <pthread.h>
#include "linkedList.hpp"
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 5
#define BALL_NUM 200

typedef struct {
    dev_fb *fb;
    Ball *ball;
} ThreadArgs;

extern int data_available;
extern char buffer[BUFFER_SIZE];
extern ThreadArgs *args[BALL_NUM];

// 공 속도 변수
extern int speed;
extern pthread_cond_t buffer_cond;
extern pthread_mutex_t buffer_mutex;
extern pthread_cond_t ball_cond;
extern pthread_mutex_t ball_mutex;

void *inputCMD(void *arg);
void *processCMD(void *arg);
void cleanup(void *arg);
void* ball_thread_func(void *arg);

#endif // THREAD_H
