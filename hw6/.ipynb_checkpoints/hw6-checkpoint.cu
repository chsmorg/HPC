#include "hw6.h"
using namespace std::chrono;





__device__ bool checkRight(Displacement * nodes, int gens, int col, int index){

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


__device__ bool checkLeft(Displacement * nodes, int gens, int col, int index){

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


__device__ void propagate(Displacement* nodes, double x, double y, double theta, int index,int col){

    for(int i = index-1; i > -1; i--){
        if(theta != 90){
            nodes[col+i].x += x;
        }
        nodes[col+i].y += y;

    }
}

__device__ void  growCol(Displacement * nodes, int col, int index){
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

__global__ void grow(Displacement* tree, int N, int generations, Block* blocks, int block_size){
    
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    
    if(x < block_size){
        int start = blocks[x].start;
        int end = blocks[x].end;
        
        
        for(unsigned int i = 7; i < generations; i++){
        for(unsigned int j = start; j < end ; j++) growCol(tree, generations*j,i);
        
        for(unsigned int j = start; j < end ; j++){
            if(tree[generations*j+i-1].theta < 90 && j < end-1){
                checkRight(tree, generations,j,i);
            }
            else if( j > start && tree[generations*j+i-1].theta > 90) {
                checkLeft(tree, generations,j,i);
            }
            
        }
    }
    }
    
}

__global__ void findIndexes(Displacement* tree, int N, int generations, Block* blocks, int block_size, int* indexes){
    
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    
    if(x < block_size){
        int start = blocks[x].start;
        int end = blocks[x].end;
        int range = end-start;
        int index = 0;
        for(unsigned int i = 7; i < generations; i++){
        for(unsigned int j = start; j < end ; j++) growCol(tree, generations*j,i);

        for(unsigned int j = start; j < end ; j++){
            
            if(indexes[j] != 0) continue;
            bool col = false;
            if(tree[generations*j+i-1].theta < 90 && j < end-1){
                col = checkRight(tree, generations,j,i);
            }
            else if( j > start && tree[generations*j+i-1].theta > 90) {
                col =checkLeft(tree, generations,j,i);
            }
            if(j ==0 || j == N-1){
                indexes[j] = -1;
                index++;
            }
            
            if(col){
                indexes[j] = i;
                index++;
            }
                
        }
          if(index == range) break;
    }
    }
    
}

__global__ void growFullTree(Displacement* tree, int N, int generations, int* indexes){
    
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    
    if(x < N && y < generations && y > 6){
        int in = generations*x+y;
        double theta;
         double dx = tree[generations*x].x - tree[generations*x+1].x;
        if(x>indexes[x]){
            theta = 90;
            tree[in].x = tree[generations*x].x + indexes[x]*dx;
        }
        else if(x<indexes[x] || indexes[x]<0){
           tree[in].x = tree[generations*x].x + (y*dx);
           theta = tree[generations*x+5].theta;
           
       }
        double dy = tree[generations*x+5].y;
        tree[in].theta = theta;
       
       tree[in].y = dy*(y);
       tree[in].col = x;
       tree[in].gen = generations-y+6;
    }
    //tree[generations*x+y].col = x;
    //tree[generations*x+y].gen = y;
   }
  


