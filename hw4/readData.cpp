//
// Created by chase morgan on 9/18/22.
//

#include "hw4.h"

void writeTreeToCsvList(Displacement ** tree, int N,int gens, std::vector<std::pair<int,int>> blocks) {
    auto s = std::to_string(gens);
    auto n = std::to_string(N);
    std::string f = s+"_"+n+"_Coord.csv";
    std::cout <<f.c_str()<<std::endl;

    FILE *fp = fopen(f.c_str(), "w");
    if (!fp) {
        printf("Failed to open file\n");
        return;
    }
    for (int i = 0; i < blocks.size(); i++) {
        for(int j = 0; j<(blocks[i].second-blocks[i].first)*gens;j++){
            fprintf(fp,"%e,%e,%d\n",tree[i][j].x,tree[i][j].y,i);
        }

    }
    fclose(fp);
    return;
}


std::vector<Displacement*> readDisplacementData(FILE *fp, int N){
    std::vector<Displacement*> nodes;
    if (!fp) {
        printf("File not found\n");
        return nodes;
    }
    int i = 0;

    while(1){

        Displacement *node = new Displacement();
        double x = 0;
        double y = 0;
        double theta = 0;
        fscanf(fp, "%lf,%lf\n",&x,&y);
        if(i/N > 0){
            theta = atan2(nodes[i-N]->y - y ,nodes[i-N]->x - x)  * 180 / M_PI;
        }
        node->x = x;
        node->y = y;
        node->col= i%N;
        node->gen = i/N;
        node->theta = theta;
        nodes.push_back(node);

        if(feof(fp)){break;}
        i++;
    }
    return nodes;
}





std::vector<int> findSplitsList( std::vector<Displacement*> tree, int N){
    std::vector<int> s;
    for(int i = N; i<N+N; i++)
        if(i!= 0 && tree[i]->theta > 90 && tree[i+1]->theta < 90) s.push_back(i-N+1);
    return s;
}




std::vector<std::pair<int,int>> createBlocksList(std::vector<Displacement*> tree ,int N, int P) {
    std::vector<int> s = findSplitsList(tree,N);
    int tasks = 100;
    //MPI_Comm_size(MPI_COMM_WORLD,&tasks);

    int chunk;
    int processes = P;
    int index = 0;
    int size = s.size();
    if(P>tasks-1){
        processes = tasks-1;
    }
    if (processes > s.size()) {
        chunk = 1;
        processes = size;
    }

    if (P < 1) {
        processes = 1;
    }
    std::vector<std::pair<int, int>> blocks;

    chunk = round((double) size / (double) (processes));
    int i = chunk;

    while (1) {
        chunk = round((double) size / (double) (processes));
        size -= chunk;

        if (processes > 1) {
            std::pair<int, int> p;
            p.first = index;
            p.second = s[i - 1];
            index = s[i - 1];
            blocks.push_back(p);
            i += chunk;
            processes--;
        } else {
            std::pair<int, int> p;
            p.first = index;
            p.second = N;
            blocks.push_back(p);
            break;
        }


    }
    return blocks;
}
Displacement ** createBlockedArray(std::vector<Displacement*> tree, int N, int P,int gens){
    std::vector<std::pair<int,int>> blocks = createBlocksList(tree,N,P);
    int size = tree.size();
    Displacement ** arr = (Displacement**)malloc(sizeof(Displacement*)*blocks.size());
    for(int i = 0; i<blocks.size(); i++){
        int cols = blocks[i].second - blocks[i].first;
        Displacement * blockArr = (Displacement*)malloc(sizeof(Displacement)*gens*cols);
        int colindex = 0;
        for(int j = blocks[i].first; j< blocks[i].second; j++){
            int l = 0;
            for(int k = 0; k<size; k+=N){
                int index = gens*colindex+l;

                blockArr[index].x = tree[k+j]->x;
                blockArr[index].y = tree[k+j]->y;
                blockArr[index].theta = tree[k+j]->theta;
                blockArr[index].col = tree[k+j]->col;
                blockArr[index].gen = tree[k+j]->gen;
                Displacement test = blockArr[index];
                Displacement * test2 = tree[k+j];
                l++;

            }
            colindex++;
        }
        arr[i] = blockArr;
    }
    for(int i = 0; i<tree.size(); i++){
        free(tree[i]);
    }
    return arr;

}

Displacement * createArray(std::vector<Displacement*> tree, int N,int gens){
    int size = tree.size();
    Displacement * arr = (Displacement*)malloc(sizeof(Displacement)*gens*N);
    for(int i = 0; i<N; i++){
        int l = 0;
        for(int j = 0; j< size; j+=N){

            int index = gens*i+l;

            arr[index].x = tree[j+i]->x;
            arr[index].y = tree[j+i]->y;
            arr[index].theta = tree[j+i]->theta;
            arr[index].col = tree[j+i]->col;
            arr[index].gen = tree[j+i]->gen;

            l++;
        }
    }
    for(int i = 0; i<tree.size(); i++){
        free(tree[i]);
    }
    return arr;

}



void freeBlockedArray(Displacement ** tree, int size){
    for(int i = 0; i <size; i++){
        free(tree[i]);
    }
    free(tree);
}