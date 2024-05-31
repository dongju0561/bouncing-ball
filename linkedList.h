#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "fbDraw.h"
#include "structure.h"

typedef struct Node
{
    Ball *data;
    struct Node *next;
}node;

void appendNode(node* head, Ball* new_data);

void deleteNode(node* head, int nodeNum);

void modifyNode(node* head, node* selectedNode, Ball new_data);

void PrintInfo(node* head);

#endif