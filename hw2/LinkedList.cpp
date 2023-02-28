//
// Created by chase morgan on 10/1/22.
//
#include "hw2.h"

Displacement * createHead(){
    Displacement *node = (Displacement *)malloc(sizeof(Displacement));
    if(node == NULL)
    {
        exit(0);
    }
    node->theta = -1;
    return node;
}
Displacement * createNode(double x,double y,double theta,int gen, int col)
{
    Displacement *node = (Displacement *)malloc(sizeof(Displacement));

    if(node == NULL)
    {
        exit(0);
    }
    node->x=x;
    node->y=y;
    node->theta=theta;
    node->gen = gen;
    node->col = col;
    node->next = NULL;

    return node;
}

void addToHead(Displacement* head, double x,double y,double theta,int gen, int col)
{
    Displacement *node= createNode(x,y,theta,gen,col);
    if(node==NULL)
    {
        exit(0);
    }
    else
    {
        if(head->next == NULL) {

            head->tail = node;
        }
        node->next= head->next;
        head->next = node;




    }
}
void printList(Displacement ** tree, int col){
    Displacement * head = tree[col];
    Displacement *node = head->next;

    while(node->next){
        std::cout<<node->gen<<"   "<< node->col << "    " << node->theta<<"    " << node->x<<"    " << node->y<<  std::endl;
        node = node->next;
    }

}
void printListShm(Displacement * tree, int col){
    Displacement * head = tree+col;
    Displacement *node = head->next;

    while(node->next){
        std::cout<<node->gen<<"   "<< node->col << "    " << node->theta<<"    " << node->x<<"    " << node->y<<  std::endl;
        node = node->next;
    }

}
