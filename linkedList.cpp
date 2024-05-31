#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "linkedList.h"
#include "fbDraw.h"


void appendNode(node* head, Ball* new_data) {
    //노드 메모리 할당
    node* new_node = (node*)malloc(sizeof(node));
    //노드 내 데이터(_tPersonalInfo타입) 메모리 할당
    new_node->data = (Ball*)malloc(sizeof(Ball));

    if (new_node->data == NULL)
    {
        printf("Memory allocation failed\n");
        free(new_node);
        return ;
    }

    //입력 받은 데이터 노드 데이터 영역에 패치
    new_node->data->pos = new_data->pos;
    new_node->data->speed = new_data->speed;
    new_node->next = NULL;

    //linked list에 노드 추가
    node* last = head;

    if (last->next == NULL) {
        last->next = new_node;
        return;
    }

    //끝에 노드 탐색
    while (last->next != NULL) {
        last = last->next;
    }
    //탐색 후 새로운 노드 추가
    last->next = new_node;
}

void deleteNode(node* head, int nodeNum) {
    node* preNode = (node*)malloc(sizeof(node));
    node* selectedNode = (node*)malloc(sizeof(node));
    *selectedNode = *head;
    FILE* file_pointer;

    //노드번호와 맵핑된 노드 탐색
    for (int i = 1; i <= nodeNum; i++)
    {
        preNode = selectedNode;
        selectedNode = selectedNode->next;
    }

    //탐색한 노드의 "직전 노드"와 "다음 노드" 연결
    
    //다음 노드가 NULL인 경우
    if (selectedNode->next == NULL)
    {
        preNode->next = NULL;
    }
    else 
    {
        preNode->next = selectedNode->next;
    }

    free(selectedNode->data);
    free(selectedNode);   
}

void PrintInfo(node* head){
    int cnt = 0;
    node* last = (node*)malloc(sizeof(node));

    if (head == NULL) {
        printf("list is empty");
        return;
    }

    *last = *head->next;
    while (last != NULL) {
        cnt += 1;
        int dx = last->data->speed.dx * last->data->speed.speed;
        int dy = last->data->speed.dy * last->data->speed.speed;
        printf("노드번호: #%d\n",cnt);        
        printf("x: %d, y: %d\n",last->data->pos.x,last->data->pos.y);
        printf("speed: dx = %d dy = %d\n",dx, dy);


        last = last->next;
    }
}

// void modifyNode(node* head, node* selectedNode, ball new_data) {

//     //수정할 데이터 입력
//     printf("-----------------------------\n");
//     printf("수정할 값을 입력하세요.\n");
//     printf("나이를 입력하세요: ");
//     scanf("%d", &nAge);
//     printf("이름를 입력하세요: ");
//     scanf("%s", szName);
//     printf("번호를 입력하세요: ");
//     scanf("%s", szPhone);

//     //입력 받은 데이터 노드 데이터 영역에 패치
//     selectedNode->data->nAge = nAge;
//     snprintf(selectedNode->data->szName, sizeof(selectedNode->data->szName), "%s", szName); 
//     snprintf(selectedNode->data->szPhone, sizeof(selectedNode->data->szPhone), "%s", szPhone);
// }

