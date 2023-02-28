//
// Created by chase morgan on 10/1/22.
//
#include "hw3.h"

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
    long double theta = head->next->theta;


    while(node){
        double nx = node->x * sin(theta*( M_PI/180));
        double x = node->next->x;
        std::cout<<node->gen<<"   "<< node->col << "    " << node->theta<<"    " << node->x<<"    " << node->y<<  std::endl;
        node = node->next;
    }

}

void freeList(Displacement ** tree, int N){
    for(int i = 0; i<N; i++){
        Displacement * n;
        Displacement * head = tree[i];
        while(head){
            n = head;
            head = head->next;
            free(n);
        }
    }
    free(tree);
}

