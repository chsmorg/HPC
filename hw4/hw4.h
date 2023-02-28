//
// Created by chase morgan on 9/18/22.
//

#ifndef UNTITLED_HW4_H
#define UNTITLED_HW4_H

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
//#include <mpi.h>

typedef struct displacementData {
    double x;
    double y;
    double theta;
    int gen;
    int col;
}Displacement;

//void defineNode(MPI_Datatype *node);
void freeBlockedArray(Displacement ** tree, int size);
void growTree(Displacement* tree, int generations, int N, int size);
void writeTreeToCsvList(Displacement ** tree, int N,int gens, std::vector<std::pair<int,int>> blocks);
Displacement * createArray(std::vector<Displacement*> tree, int N,int gens);
std::vector<std::pair<int,int>> createBlocksList(std::vector<Displacement*> tree, int N, int P);
Displacement ** createBlockedArray(std::vector<Displacement*> tree, int N, int P,int gens);
std::vector<Displacement*> readDisplacementData(FILE *fp, int N);
std::vector<Displacement*> readDisplacementData(FILE *fp, int N);
std::vector<int> findSplitsList( std::vector<std::vector<Displacement*>> tree, int N);



#endif //UNTITLED_HW4_H
