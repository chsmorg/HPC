//
// Created by chase morgan on 9/18/22.
//

#ifndef UNTITLED_HW3_H
#define UNTITLED_HW3_H

#include <unistd.h>
#include <cstdlib>

#include <cmath>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include <pthread.h>

typedef struct displacementData {
    double x;
    double y;
    double theta;
    int gen;
    int col;
    displacementData * next = NULL;
    displacementData * tail = NULL;
}Displacement;

typedef struct {
    Displacement ** tree;
    int gens;
    int start;
    int end;
    //int threads;
    //int part;
   // pthread_mutex_t lock;

}Data_t;


std::vector<Displacement*> readDisplacementData(FILE *fp, int N);

//draft functions
std::vector<std::vector<Displacement*>> convertToTree(std::vector<Displacement*>, int N);
void growTree(std::vector<std::vector<Displacement*>> &tree, int generations);
void growCol(std::vector<Displacement*> &nodes);
void writeTreeToCsv(std::vector<std::vector<Displacement*>> tree, int gens,int N);

//linked list functions
void addToHead(Displacement* head, double x,double y,double theta,int gen, int col);
Displacement * createHead();
void printList(Displacement ** tree, int col);


//threaded functions
std::vector<std::pair<int,int>> createBlocksList(Displacement **tree,int N, int P);
Displacement ** convertToList(std::vector<Displacement*> nodes, int N);
void* growTreeBlockedList(void *data);
void writeTreeToCsvList(Displacement ** tree, int N,int gens);
void freeList(Displacement ** tree, int N);



#endif //UNTITLED_HW3_H
