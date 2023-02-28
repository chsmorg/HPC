//
// Created by chase morgan on 9/18/22.
//

#ifndef UNTITLED_HW4_H
#define UNTITLED_HW4_H

#include <unistd.h>
#include <cstdlib>

#include <cmath>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <cuda_runtime.h>
#include "helper_image.h"


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
}Displacement;

typedef struct Blocks{
    int start;
    int end;
}Block;

void growTree(Displacement* tree, int generations, int N, int size);
void writeTreeToCsvList(Displacement * tree, int N,int gens);


std::vector<std::pair<int,int>> createBlocksList(std::vector<Displacement*> tree, int N);
Displacement* createArray(std::vector<Displacement*> tree, int N,int gens);
Block* createBlocks(std::vector<std::pair<int,int>> b);

std::vector<Displacement*> readDisplacementData(FILE *fp, int N);
std::vector<Displacement*> readDisplacementData(FILE *fp, int N);
std::vector<int> findSplitsList( std::vector<std::vector<Displacement*>> tree, int N);

__global__ void grow(Displacement* image, int N, int generations, Block* blocks, int block_size);
__global__ void findIndexes(Displacement* tree, int N, int generations, Block* blocks, int block_size, int* indexes);
__global__ void growFullTree(Displacement* tree, int N, int generations, int* indexes);

void growCpu(Displacement* tree, int N, int generations, Block* blocks, int block_size);



#endif //UNTITLED_HW4_H
