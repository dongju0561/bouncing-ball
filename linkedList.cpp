#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "linkedList.hpp"
#include "fbDraw.hpp"

node *head = (node *)malloc(sizeof(node));

void append_node(node* head, Ball* new_data) 
{
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

    if (last->next == NULL) 
    {
        last->next = new_node;
        return;
    }

    //끝에 노드 탐색
    while (last->next != NULL)
        last = last->next;

    //탐색 후 새로운 노드 추가
    last->next = new_node;
}

void delete_last_node(node* head) {
    if (head->next == NULL)
    {
        return;
    }
    node* preNode = (node*)malloc(sizeof(node));
    node* selectedNode = (node*)malloc(sizeof(node));
    *preNode = *head;
    *selectedNode = *head->next;
    FILE* file_pointer;

    //마지막 노드 탐색
    while (selectedNode->next != NULL)
    {
        preNode = selectedNode;
        selectedNode = selectedNode->next;
    }
    preNode->next = NULL;

    free(selectedNode->data);
    free(selectedNode);   
}

void PrintInfo(node* head)
{
    int cnt = 0;
    node* last = (node*)malloc(sizeof(node));

    if (head->next == NULL) 
    {
        printf("list is empty");
        return;
    }

    *last = *head->next;
    while (last != NULL) 
    {
        cnt += 1;
        int dx = last->data->speed.dx * last->data->speed.speed;
        int dy = last->data->speed.dy * last->data->speed.speed;
        printf("노드번호: #%d\n",cnt);        
        printf("x: %d, y: %d\n",last->data->pos.x,last->data->pos.y);
        printf("speed: dx = %d dy = %d\n",dx, dy);


        last = last->next;
    }
}

void close_list(node* head)
{
    node* selectedNode = (node*)malloc(sizeof(node));
    node* nextedNode = (node*)malloc(sizeof(node));
    if (head->next != NULL) 
    {
        *selectedNode = *head->next;
        *nextedNode = *head->next;
        while (nextedNode->next != NULL) 
        {
            nextedNode = selectedNode->next;
            free(selectedNode->data);
            free(selectedNode);
            selectedNode = nextedNode;
        }
    }
    free(head->data);
    free(head->next);
    free(head);

    exit(1);
}