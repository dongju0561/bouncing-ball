#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "fbDraw.hpp"
#include "common.hpp"
#include "linkedList.hpp"

extern node *head;

//노드를 추가하는 함수
void appendNode(node* head, Ball* new_data);
//추가된 노드 정보를 출력하는 함수
void PrintInfo(node* head);
//리스트를 순회하면서 노드들의 메모리 할당을 해제하는 함수
void close_list(node* head);

#endif // LINKEDLIST_H