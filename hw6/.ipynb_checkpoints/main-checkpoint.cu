using namespace std;
#include "hw6.h"

void print_arrayAt(Displacement* arr, int col, int gens){
    
   int j = col*gens;
    
    for(int i = 0; i <gens; i++){
        int k = j+i;
        std::cout<<"gen: "<<arr[k].gen<<"  col: "<<arr[k].col<<"  theta: "<<arr[k].theta<<"  x: "<<arr[k].x<<"  y: "<<arr[k].y<<std::endl;
    }
    return;
    
}

Displacement* serialCPU(int N, int gens, FILE *fp){
        std::vector<Displacement*> vec1 = readDisplacementData(fp,N);
        std::vector<std::pair<int,int>> blocked = createBlocksList(vec1,N);
        Displacement* m = createArray(vec1,N,gens);
        Block *b = createBlocks(blocked);
        int block_size = blocked.size();
    
        auto start = chrono::high_resolution_clock::now();
        std::cout<<"starting serial CPU Growth..."<<std::endl;
    
        growCpu(m,N,gens,b,block_size);
    
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    
        std::cout << "Time taken to Grow Serial on CPU: "
                  << duration.count() << " Milliseconds" << std::endl;
        
        free(b);
        fclose(fp);
        writeTreeToCsvList(m,N,gens);
    
       // print_arrayAt(m,50,gens);
        
        return m;
    
    
}


Displacement* serialGPU(int N, int gens,FILE *fp){
    
    
        std::vector<Displacement*> vec1 = readDisplacementData(fp,N);
        std::vector<std::pair<int,int>> blocked = createBlocksList(vec1,N);
        Block *b = createBlocks(blocked);
        int block_size = blocked.size();
    
        Displacement* array;
        Block* block_array;
    
    
        Displacement* m = createArray(vec1,N,gens);
    
        cudaMalloc((void**)&array, sizeof(Displacement)*gens*N);
        cudaMalloc((void**)&block_array, sizeof(Block)*block_size);
    
    
        cudaMemcpy(array, m, sizeof(Displacement)*gens*N, cudaMemcpyHostToDevice);
        cudaMemcpy(block_array, b, sizeof(Block)*block_size, cudaMemcpyHostToDevice);
    
        dim3 threads(1, 1);
        dim3 blocks(block_size, 1);
    
    
        auto start = chrono::high_resolution_clock::now();
        std::cout<<"starting serial GPU Growth..."<<std::endl;
        grow<<<blocks, threads>>>(array, N, gens, block_array, block_size);
    
        cudaDeviceSynchronize();
    
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        std::cout << "Time taken to Grow Serial on GPU: "
                  << duration.count() << " Milliseconds" << std::endl;
        cudaMemcpy(m, array, sizeof(Displacement)*gens*N, cudaMemcpyDeviceToHost);
    
        //write serial to output file:
    
        //writeTreeToCsvList(m,N,gens);
    
        
    
    free(b);
    cudaFree(array);
    cudaFree(block_array);
    fclose(fp);
    return m;
    
    
}

int* locateIndexes(std::vector<Displacement*> vec1, int N, int gens, Displacement* m){
    
        std::vector<std::pair<int,int>> blocked = createBlocksList(vec1,N);
        Block *b = createBlocks(blocked);
        Displacement* array;
        Block* block_array;
        int* indexes;
    
        int temp_gens = 10;
        int block_size = blocked.size();
        int* col_indexes = (int*)malloc(sizeof(int)*N);
        cudaMalloc((void**)&array, sizeof(Displacement)*gens*N);
        cudaMalloc((void**)&block_array, sizeof(Block)*block_size);
        cudaMalloc((void**)&indexes, sizeof(int)*N);
    
        cudaMemcpy(array, m, sizeof(Displacement)*gens*N, cudaMemcpyHostToDevice);
        cudaMemcpy(block_array, b, sizeof(Block)*block_size, cudaMemcpyHostToDevice);
    
        dim3 blocks(1, 1);
        dim3 grid(block_size, 1);
        auto start = chrono::high_resolution_clock::now();
    
        findIndexes<<<grid, blocks>>>(array, N, temp_gens, block_array, block_size,indexes);
    
        cudaDeviceSynchronize();
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        std::cout << "Time taken to Find Indexes on GPU: "
                  << duration.count() << " Milliseconds" << std::endl;
        cudaMemcpy(col_indexes, indexes, sizeof(int)*N, cudaMemcpyDeviceToHost);
    
        cudaFree(array);
        cudaFree(block_array);
        cudaFree(indexes);
        free(b);

        return col_indexes;
    
        
    
}

Displacement* parallelGPU(int N, int gens, int P, FILE *fp){
    
        std::vector<Displacement*> vec1 = readDisplacementData(fp,N);
        
    
        Displacement* array;
        int* indexes;
    
         Displacement* m = createArray(vec1,N,gens);
         int* col_indexes = locateIndexes(vec1,N,gens,m);
        cudaMalloc((void**)&array, sizeof(Displacement)*gens*N);
      
        cudaMalloc((void**)&indexes, sizeof(int)*N);
        
    
    
        cudaMemcpy(array, m, sizeof(Displacement)*gens*N, cudaMemcpyHostToDevice);
  
        cudaMemcpy(indexes, col_indexes, sizeof(int)*N, cudaMemcpyHostToDevice);
 
        
    
        
                            
 
    

    
    
        std::cout<<"starting Parallel GPU Growth..."<<std::endl;
        auto start = chrono::high_resolution_clock::now();

    
    if(P == 1){
                dim3 gridDim(N,gens/N);
                dim3 blockDim(1,N);
                growFullTree<<<gridDim,blockDim>>>(array,N,gens,indexes); 
        }
    else if(P == 2){
                dim3 gridDim(N,N);
                dim3 blockDim(1,gens/N);
                growFullTree<<<gridDim,blockDim>>>(array,N,gens,indexes); 
        }
    else if(P==3){
                dim3 gridDim(N,N/2);
                dim3 blockDim(1,round(gens/(N/2)));
                growFullTree<<<gridDim,blockDim>>>(array,N,gens,indexes); 
    }
    else if(P==4){
                dim3 gridDim(N,N/4);
                dim3 blockDim(1,round(gens/(N/4)));
                growFullTree<<<gridDim,blockDim>>>(array,N,gens,indexes); 
    }
    else{
                std::cout<<"invalid Dim size, range 1-4. look at readMe for more info. Dim 1 being used"<<std::endl;
                dim3 gridDim(N,gens/N);
                dim3 blockDim(1,N);
                growFullTree<<<gridDim,blockDim>>>(array,N,gens,indexes); 
    }
         
        cudaDeviceSynchronize();
    
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

        std::cout << "Time taken to Grow Parallel on GPU: "
                  << duration.count() << " Nanoseconds" << std::endl;
    
        cudaMemcpy(m, array, sizeof(Displacement)*gens*N, cudaMemcpyDeviceToHost);
    
        writeTreeToCsvList(m,N,gens);
    
        //print_arrayAt(m,50,gens);
    
   
    
        
        free(col_indexes);
        cudaFree(array);
        cudaFree(indexes);
        fclose(fp);
    return m;
}


void compare(Displacement* arr1, Displacement* arr2, int N, int gens){
    float not_matching = 0;
    
    for(int i = 0; i<N; i++){
        for(int j = 0; j<gens; j++){
            if(arr1[i*gens +j].x != arr2[i*gens+j].x){
                not_matching += 1;
            }
        }
    }
    float p = ((gens*N)-not_matching) / (gens*N) * 100;
    std::cout<<"total matching: "<<p<<"%"<<std::endl;
    
}



int main(int argc, char* argv[]){
    int N = atoi(argv[2]);
    int gens = atoi(argv[3]);
    int P = atoi(argv[4]);
    
    FILE *fp;
    fp = fopen(argv[1],"r");
    if(!fp){
        printf("File not found\n");
        exit(0);
    }
    //this the fast one
    Displacement* gpu = parallelGPU(N,gens,P,fp);
    
    //this the slow one
   // Displacement* gpu = serialGPU(N,gens,fp);
    //fp = fopen(argv[1],"r");
    
    //this the normal one
  // Displacement* cpu = serialCPU(N,gens,fp);
    
    //compare(cpu,gpu,N,gens);
    
    free(gpu);
    free(cpu);

}
