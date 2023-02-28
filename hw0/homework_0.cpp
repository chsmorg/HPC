//
// Created by chase morgan on 8/30/22.
//

#include "homework_0.h"
using namespace std::chrono;

std::vector<Displacement> readDisplacementData(FILE *fp, int N) {
    auto start = high_resolution_clock::now();
    std::vector<Displacement> nodes;
    if (!fp) {
        printf("File not found\n");
        return nodes;
    }
    int i = 0;
    while(1){
        nodes.push_back(Displacement());
        fscanf(fp, "%lf\n%lf\n%lf\n",&nodes[i].x,&nodes[i].y,&nodes[i].theta);
        if(feof(fp)){break;}
        i++;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken to parse displacement file: "
              << duration.count() << " microseconds" << std::endl;
    std::cout << "Number of nodes: " << nodes.size() << std::endl;
    writeCsv(nodes);
   return nodes;
}

void writeCsv(std::vector<Displacement> n){
    FILE *fp = fopen("displacements.csv","w");
    if(!fp){
        printf("Failed to open file\n");
        return;
    }
    for(int i = 0; i<n.size(); i++){
        fprintf(fp,"%e\n%e\n%e\n",n[i].x,n[i].y,n[i].theta);
    }
    return;

}

std::vector<Position> readPositionData(FILE *fp, int N){
    auto start = high_resolution_clock::now();
    std::vector<Position> nodes;
    if (!fp) {
        printf("File not found\n");
        return nodes;
    }
    int i = 0;
    while(1){
        nodes.push_back(Position());
        fscanf(fp, "%lf,%lf\n",&nodes[i].x,&nodes[i].y);
        nodes[i].col = i%N;
        nodes[i].row = i/N;
        if(feof(fp)){break;}
        i++;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken to parse displacement file: "
              << duration.count() << " microseconds" << std::endl;
    std::cout << "Number of nodes: " << nodes.size() << std::endl;
    return nodes;
}

Sum calcStiffnessData(FILE *fp, int rows, int cols){
    double** m = readMatrix(fp,rows,cols);
    auto start = high_resolution_clock::now();

    Sum *data = new Sum;
    data->colsSum = (double*)malloc(sizeof(double)*cols);
    data->rowsSum = (double*)malloc(sizeof(double)*rows);

    for(int i = 0; i<rows; i++){
        for(int j = 0; j< cols; j++){
            data->rowsSum[j] +=m[i][j];
            data->colsSum[i] +=m[i][j];
            data->total +=m[i][j];
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken to sum matrices: "
              << duration.count() << " microseconds" << std::endl;
    return *data;


}


double** makeMatrix(int rows, int cols){
    double** M = new double *[rows];
    for(int i = 0; i < rows; i++) {
        M[i] = new double[cols];
        for(int j = 0; j < cols; j++) {
            M[i][j] = 0;
        }
    }
    return M;
}

double** readMatrix(FILE* fp, int rows, int cols){
    if (!fp) {
        printf("File not found\n");
        return NULL;
    }
    double** m = makeMatrix(rows, cols);

    auto start = high_resolution_clock::now();



    for(int i = 0; i<rows;i++){
        for(int j =0; j<cols;j++){
            fscanf(fp,"%lf,",&m[i][j]);
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken to parse Matrix File: "
              << duration.count() << " microseconds" << std::endl;

    return m;
}


