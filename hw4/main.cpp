#include <iostream>
#include "hw4.h"
using namespace std::chrono;


int main(int argc, char **argv) {

//    int	taskid,
//            numtasks;
//
//
//    MPI_Init(&argc,&argv);
//    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
//    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
//    MPI_Datatype node;
//    defineNode(&node);


//    if(taskid == 0){
        int N = atoi(argv[2]);
        int gens = atoi(argv[3]);
        int T = atoi(argv[4])-1 ;


        FILE *fp;
        fp = fopen(argv[1],"r");
        if(!fp){
            printf("File not found\n");
            exit(0);
        }

        std::vector<Displacement*> vec1 = readDisplacementData(fp,N);

        std::vector<std::pair<int,int>> blocks = createBlocksList(vec1,N,T);
       // Displacement ** m = createBlockedArray(vec1,N,T,gens);
        Displacement * b = createArray(vec1,N,gens);
        std::cout<<blocks.size()<<std::endl;

        int col = 2;
        for(int i = 0; i<blocks.size(); i++){
            std::cout<<blocks[i].first<<"  "<<blocks[i].second<<std::endl;
        }


//        std::cout<<"Starting growth..."<<std::endl;
//
//
//        auto start = high_resolution_clock::now();
//
//        for(int i = 0; i<blocks.size(); i++){
//            int size = gens*(blocks[i].second - blocks[i].first);
//
//            int index[4] = {blocks[i].first, blocks[i].second,gens,N};
//
//            MPI_Send(index, 4, MPI_INT, i+1,0,MPI_COMM_WORLD);
//            MPI_Send(m[i], size, node, i+1, 0, MPI_COMM_WORLD);
//        }
//        if(numtasks>blocks.size()){
//            for(int i = blocks.size(); i < numtasks; i++){
//                int index[4] = {0,0,0,0};
//
//                MPI_Send(index, 4, MPI_INT, i,0,MPI_COMM_WORLD);
//            }
//        }
//        MPI_Barrier(MPI_COMM_WORLD);
//
//        auto stop = high_resolution_clock::now();
//        auto duration = duration_cast<milliseconds>(stop - start);
//
//        std::cout << "Time taken to grow CNT "<<gens<<" generations: "
//                  << duration.count() << " milliseconds with "<<blocks.size()<<" workers" << std::endl;
//
//        for(int i = 0; i<blocks.size(); i++){
//            int size = gens*(blocks[i].second - blocks[i].first);
//            MPI_Recv(m[i], size, node, i+1, 0, MPI_COMM_WORLD,
//                     MPI_STATUS_IGNORE);
//        }
//        writeTreeToCsvList(m,N,gens,blocks);
//        freeBlockedArray(m,blocks.size());
//        fclose(fp);
//
//    }
//
//    else{
//
//        int index[4];
//        MPI_Recv(index, 4, MPI_INT, 0, 0, MPI_COMM_WORLD,
//                 MPI_STATUS_IGNORE);
//        if(index[3] != 0){
//            int size = (index[1]-index[0]);
//            int gens = index[2];
//            int N = index[3];
//
//            Displacement * data = (Displacement*)malloc(sizeof(Displacement)*size*gens);
//
//            MPI_Recv(data, size*gens, node, 0, 0, MPI_COMM_WORLD,
//                     MPI_STATUS_IGNORE);
//            growTree(data, gens, N, size);
//            MPI_Barrier(MPI_COMM_WORLD);
//            MPI_Send(data, size*gens, node, 0, 0, MPI_COMM_WORLD);
//            free(data);
//        }else{
//            MPI_Barrier(MPI_COMM_WORLD);
//        }
//
//    }
//
//
//    MPI_Finalize();


}



