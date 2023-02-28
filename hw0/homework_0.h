//
// Created by chase morgan on 8/30/22.
//

#ifndef HW0_HOMEWORK_0_H
#define HW0_HOMEWORK_0_H


#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>

typedef struct displacementData {
    double x;
    double y;
    double theta;
}Displacement;

typedef struct positionData  {
    double x;
    double y;
    int row;
    int col;
}Position;

typedef struct MatrixSum   {
    double* rowsSum;
    double* colsSum;
    double total = 0;
}Sum;
double** makeMatrix(int rows, int cols);
double** readMatrix(FILE *fp, int rows, int cols);
std::vector<Displacement> readDisplacementData(FILE *fp, int N);
std::vector<Position> readPositionData(FILE *fp, int N);
void writeCsv(std::vector<Displacement> n);
Sum calcStiffnessData(FILE *fp, int rows, int cols);
#endif //HW0_HOMEWORK_0_H
