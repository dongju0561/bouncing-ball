# bouncing-ball

## Introduction
공 객체를 생성하고 공의 위치 데이터에 따라 프레임버퍼 상에 출력하는 프로그램입니다.

## Tech stack
- 프레임버퍼
- 스레드 : POSIX 스레드(pthread)를 사용하여 다중 스레드 구현

## Feature
스크린에 벽에 맞고 튕기는 공을 생성('a'), 삭제('z')
공의 속도 증가('s')
공의 속도 감소('d')
프로그램 종료('x')

## Installation

### Installing

1. 프로젝트 클론:
    ```sh
    git clone https://github.com/dongju0561/frameBuffer.git
    ```
2. 프로젝트 디렉토리로 이동:
    ```sh
    cd frameBuffer
    ```
3. 프로그램 빌드
    ```sh
    make
    ```

# Example command to run the project
- ctrl + Shift + F2~F5 입력(가상 콘솔로 이동)
- 프로젝트 디렉토리로 이동:
```sh
cd frameBuffer
```
- 빌드한 파일 실행
```sh
sudo ./fb
```
