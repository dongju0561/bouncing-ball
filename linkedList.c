#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "linkedList.h"

void appendNode(struct node* head, struct _tPersonalInfo new_data) {

    //노드 메모리 할당
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    //노드 내 데이터(_tPersonalInfo타입) 메모리 할당
    new_node->data = (struct _tPersonalInfo*)malloc(sizeof(struct _tPersonalInfo));

    if (new_node->data == NULL)
    {
        printf("Memory allocation failed\n");
        free(new_node);
        return ;
    }

    //입력 받은 데이터 노드 데이터 영역에 패치
    new_node->data->nAge = new_data.nAge;
    snprintf(new_node->data->szName, sizeof(new_node->data->szName), "%s", new_data.szName);
    snprintf(new_node->data->szPhone, sizeof(new_node->data->szPhone), "%s", new_data.szPhone);
    new_node->next = NULL;

    //linked list에 노드 추가
    struct node* last = head;

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

void deleteNode(struct node* head, int nodeNum) {
    struct node* preNode = (struct node*)malloc(sizeof(struct node));
    struct node* selectedNode = (struct node*)malloc(sizeof(struct node));
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
void modifyNode(struct node* head, struct node* selectedNode, struct _tPersonalInfo new_data) {
    int nAge = 0;
    char szName[MAX_NAME_LENGTH];
    char szPhone[MAX_PHONE_LENGTH];

    //수정할 데이터 입력
    printf("-----------------------------\n");
    printf("수정할 값을 입력하세요.\n");
    printf("나이를 입력하세요: ");
    scanf("%d", &nAge);
    printf("이름를 입력하세요: ");
    scanf("%s", szName);
    printf("번호를 입력하세요: ");
    scanf("%s", szPhone);

    //입력 받은 데이터 노드 데이터 영역에 패치
    selectedNode->data->nAge = nAge;
    snprintf(selectedNode->data->szName, sizeof(selectedNode->data->szName), "%s", szName); 
    snprintf(selectedNode->data->szPhone, sizeof(selectedNode->data->szPhone), "%s", szPhone);
}

