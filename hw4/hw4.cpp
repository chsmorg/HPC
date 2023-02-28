//
// Created by chase morgan on 9/18/22.
//

#include "hw4.h"
using namespace std::chrono;



void defineNode(MPI_Datatype *node) {
    const int count = 5;
    int blocklens[count] = {1,1,1,1,1};
    MPI_Datatype types[count] = {MPI_DOUBLE, MPI_DOUBLE,MPI_DOUBLE,MPI_INT,MPI_INT};
    MPI_Aint disps[count] = {offsetof(Displacement,x),
                             offsetof(Displacement,y),
                             offsetof(Displacement,theta),
                             offsetof(Displacement,gen),
                             offsetof(Displacement,col)};

    MPI_Type_create_struct(count, blocklens, disps, types, node);
    MPI_Type_commit(node);
}

void checkRight(Displacement * nodes, int gens, int col, int index){

    double x1 = nodes[gens*col].x;
    Displacement *node = (nodes+gens*col);
    for(int i = 0; i<=index; i++){
        if(abs((x1) - (nodes[gens*(col+1)+i].x)) < 5e-08){
            nodes[gens*col+1].theta = 90;
            nodes[gens*col].x = nodes[gens*col+1].x;
            nodes[gens*(col+1)+1].theta = 90;
            nodes[gens*(col+1)].x = nodes[gens*(col+1)+1].x;
            return;
        }
    }
    return;


}
void checkLeft(Displacement * nodes, int gens, int col, int index){

    double x1 = nodes[gens*col].x;

    for(int i = 0; i<=index; i++){
        if(abs((x1) - (nodes[gens*(col-1)+i].x)) < 5e-08){
            nodes[gens*col+1].theta = 90;
            nodes[gens*col].x = nodes[gens*col+1].x;
            nodes[gens*(col-1)+1].theta = 90;
            nodes[gens*(col-1)].x = nodes[gens*(col-1)+1].x;
            return;
        }
    }
    return;


}
void propagate(Displacement* nodes, double x, double y, double theta, int index,int col){

    for(int i = index-1; i > -1; i--){
        if(theta != 90){
            nodes[col+i].x += x;
        }
        nodes[col+i].y += y;

    }
}


void growCol(Displacement * nodes, int col, int index){
    //create new node and assign values then add to front of col

    double y = nodes[col+index-2].y;
    double x = nodes[col+index-2].x-nodes[col+index-1].x;
    double theta = nodes[col+1].theta;

    nodes[col+index].col = nodes[col+index-1].col;

    nodes[col+index].gen = nodes[col+index-1].gen+1;
    nodes[col+index].y = 0;
    nodes[col+index].theta = theta;
    nodes[col+index].x = nodes[col+index-1].x;

    propagate(nodes,x,y,theta, index,col);

    return;
}



void growTree(Displacement* tree, int generations, int N, int size){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    auto start = high_resolution_clock::now();

    for(unsigned int i = 7; i < generations; i++){
        for(unsigned int j = 0; j < size ; j++) growCol(tree, generations*j,i);

        for(unsigned int j = 0; j < size ; j++){
            if(tree[generations*j+1].theta < 90 && j < size-1){
                checkRight(tree, generations,j,i);
            }
            else if( j > 0 && tree[generations*j+1].theta > 90) {
                checkLeft(tree, generations,j,i);
            }
        }


    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Time taken to grow and check tree to "<<generations<<" generations: "
              << duration.count() << " milliseconds from rank "<<rank <<" block size: "<<size<< std::endl;
    return;
}



