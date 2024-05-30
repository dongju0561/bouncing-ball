#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include"fbDraw.h"

typedef struct 
{
    ball *data;
    node *next;
}node;

void appendNode(node* head, ball new_data);

void deleteNode(node* head, int nodeNum);

void modifyNode(node* head, node* selectedNode, ball new_data);

#endif