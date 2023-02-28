#include "hw6.h"
using namespace std::chrono;




bool checkRightCpu(Displacement * nodes, int gens, int col, int index){

    double x1 = nodes[gens*col].x;
    // Displacement *node = (nodes+gens*col);
    for(int i = 0; i<=index; i++){
        if(abs((x1) - (nodes[gens*(col+1)+i].x)) < 5e-08){
            nodes[gens*col+1].theta = 90;
            nodes[gens*col].x = nodes[gens*col+1].x;
            nodes[gens*(col+1)+1].theta = 90;
            nodes[gens*(col+1)].x = nodes[gens*(col+1)+1].x;
            return true;
        }
    }
    return false;


}


bool checkLeftCpu(Displacement * nodes, int gens, int col, int index){

    double x1 = nodes[gens*col].x;

    for(int i = 0; i<=index; i++){
        if(abs((x1) - (nodes[gens*(col-1)+i].x)) < 5e-08){
            nodes[gens*col+1].theta = 90;
            nodes[gens*col].x = nodes[gens*col+1].x;
            nodes[gens*(col-1)+1].theta = 90;
            nodes[gens*(col-1)].x = nodes[gens*(col-1)+1].x;
            return true;
        }
    }
    return false;


}


void propagateCpu(Displacement* nodes, double x, double y, double theta, int index,int col){

    for(int i = index-1; i > -1; i--){
        if(theta != 90){
            nodes[col+i].x += x;
        }
        nodes[col+i].y += y;

    }
}

 void  growColCpu(Displacement * nodes, int col, int index){
    //create new node and assign values then add to front of col

    double y = nodes[col+index-2].y;
    double x = nodes[col+index-2].x-nodes[col+index-1].x;
    double theta = nodes[col+1].theta;

    nodes[col+index].col = nodes[col+index-1].col;

    nodes[col+index].gen = nodes[col+index-1].gen+1;
    nodes[col+index].y = 0;
    nodes[col+index].theta = theta;
    nodes[col+index].x = nodes[col+index-1].x;

    propagateCpu(nodes,x,y,theta, index,col);

    return;
}



void growCpu(Displacement* tree, int N, int generations, Block* blocks, int block_size){
    
    for(int x = 0; x<block_size; x++){
        int start = blocks[x].start;
        int end = blocks[x].end;
        
        
        for(unsigned int i = 7; i < generations; i++){
        for(unsigned int j = start; j < end ; j++) growColCpu(tree, generations*j,i);
        
        for(unsigned int j = start; j < end ; j++){
            if(tree[generations*j+i-1].theta < 90 && j < end-1){
                checkRightCpu(tree, generations,j,i);
            }
            else if( j > start && tree[generations*j+i-1].theta > 90) {
                checkLeftCpu(tree, generations,j,i);
            }
            
        }
    }
    }
    
}




