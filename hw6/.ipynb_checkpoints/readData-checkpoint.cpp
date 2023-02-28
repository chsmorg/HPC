//
// Created by chase morgan on 9/18/22.
//

#include "hw6.h"

void writeTreeToCsvList(Displacement * tree, int N,int gens) {
    auto s = std::to_string(gens);
    auto n = std::to_string(N);
    std::string f = s+"_"+n+"_Coord.csv";
    std::cout <<f.c_str()<<std::endl;

    FILE *fp = fopen(f.c_str(), "w");
    if (!fp) {
        printf("Failed to open file\n");
        return;
    }
    for (int i = 0; i < N*gens; i++) {
            fprintf(fp,"%e,%e\n",tree[i].x,tree[i].y);

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




std::vector<std::pair<int,int>> createBlocksList(std::vector<Displacement*> tree ,int N) {
    std::vector<int> s = findSplitsList(tree,N);

    int index = 0;
    int size = s.size();
    std::vector<std::pair<int, int>> blocks;

    
    
    for(int i = 1; i< size; i++){
        std::pair<int, int> p;
            p.first = index;
            p.second = s[i - 1];
            index = s[i - 1]; 
        blocks.push_back(p);
        }
    
        std::pair<int, int> p;
        p.first = index;
        p.second = N;
        blocks.push_back(p);
    
    return blocks;
}

Block* createBlocks(std::vector<std::pair<int,int>> b){
    Block * blocks = (Block*)malloc(sizeof(Block)*b.size());
    for(int i = 0; i<b.size(); i++){
        blocks[i].start = b[i].first;
        blocks[i].end = b[i].second;
    }
    return blocks;
}

Displacement* createArray(std::vector<Displacement*> tree, int N,int gens){
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