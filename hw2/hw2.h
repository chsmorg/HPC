//
// Created by chase morgan on 9/18/22.
//

#ifndef UNTITLED_HW2_H
#define UNTITLED_HW2_H
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <vector>
#include <string>

typedef struct displacementData {
    double x;
    double y;
    double theta;
    int gen;
    int col;
    displacementData * next = NULL;
    displacementData * tail = NULL;
}Displacement;


std::vector<Displacement*> readDisplacementData(FILE *fp, int N);
std::vector<std::vector<Displacement*>> convertToTree(std::vector<Displacement*>, int N);
void growTree(std::vector<std::vector<Displacement*>> &tree, int generations);
void growCol(std::vector<Displacement*> &nodes);
void propagate(std::vector<Displacement*> &nodes, double x, double y);
void writeTreeToCsv(std::vector<std::vector<Displacement*>> tree);
std::vector<std::vector<std::vector<Displacement*>>> createBlocks(std::vector<std::vector<Displacement*>> tree, std::vector<int> s, int P);
std::vector<int> findSplits(std::vector<std::vector<Displacement*>> tree, int N);
void writeTreeToCsvShm(Displacement * tree, int N);

std::vector<std::pair<int,int>> createBlocksList(Displacement **tree,int N, int P);
void addToHead(Displacement* head, double x,double y,double theta,int gen, int col);
Displacement * createHead();
Displacement ** convertToList(std::vector<Displacement*> nodes, int N);
void storeInShm(Displacement** tree, int N,Displacement * shm);
void printListShm(Displacement * tree, int col);

void blockedGrow(Displacement * shm, int generations, std::pair<int,int> p, int N);
void writeTreeToCsv(Displacement * tree, int N);
void checkShm(Displacement * head1, Displacement * head2);
void propagateShm(Displacement * head, double x, double y);
void growColShm(Displacement * shm, int index, Displacement *head);

#endif //UNTITLED_HW2_H

