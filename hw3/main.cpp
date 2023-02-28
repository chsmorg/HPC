#include <iostream>
#include "hw3.h"
using namespace std::chrono;


int main(int argc, char **argv) {
    int N = atoi(argv[2]);
    int gens = atoi(argv[3]);
    int T = atoi(argv[4]);
    int check_threads = 1;
    int block_threads;

//    if(T>=4){
//        block_threads = T/4;
//        check_threads = T - block_threads;
//    }
//    else{
//        //block_threads = T;
//    }

    FILE *fp;
    fp = fopen(argv[1],"r");
    if(!fp){
        printf("File not found\n");
        exit(0);
    }

    std::vector<Displacement*> vec1 = readDisplacementData(fp,N);
    //std::vector<std::vector<Displacement*>> vec2 = convertToTree(vec1, N);


    Displacement ** tree = convertToList(vec1,N);
    std::vector<std::pair<int,int>> blocks = createBlocksList(tree,N,T);



    //printList(tree,0);


    int num_threads = blocks.size();
    pthread_t threads[num_threads];
    std::cout<<"Starting growth..."<<std::endl;
    auto start = high_resolution_clock::now();
    std::vector<void*> data;
    for(size_t i = 0 ; i <num_threads; i++){
        data.push_back(new Data_t{tree, gens, blocks[i].first, blocks[i].second});
        pthread_create(&threads[i], NULL, growTreeBlockedList, data[i]);
    }
    for(size_t i = 0; i< num_threads; i++){
        pthread_join(threads[i], NULL);
        free(data[i]);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "time taken to grow tree "<<gens<<" times with "<<T<<" Threads: "
              << duration.count() << " milliseconds" << std::endl;

    writeTreeToCsvList(tree,N,gens);
    //printList(tree,0);
    freeList(tree,N);
    for(int i = 0; i<vec1.size(); i++){
        free(vec1[i]);
    }
    fclose(fp);
}





