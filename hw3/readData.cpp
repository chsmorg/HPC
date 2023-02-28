//
// Created by chase morgan on 9/18/22.
//

#include "hw3.h"

void writeTreeToCsv(std::vector<std::vector<Displacement*>> tree, int gens,int N){
    auto s = std::to_string(gens);
    auto n = std::to_string(N);
    std::string f = s+"_"+n+"_Coord.csv";
    std::cout <<f.c_str()<<std::endl;

    FILE *fp = fopen(f.c_str(),"w");
    if(!fp){
        printf("Failed to open file\n");
        return;
    }
    for(int i = 0; i<tree.size(); i++){
        for(int j = 0; j<tree[i].size(); j++){
            fprintf(fp,"%e,%e\n",tree[i][j]->x,tree[i][j]->y);
        }

    }
    fclose(fp);
    return;
}

void writeTreeToCsvList(Displacement ** tree, int N,int gens) {
    auto s = std::to_string(gens);
    auto n = std::to_string(N);
    std::string f = s+"_"+n+"_Coord.csv";
    std::cout <<f.c_str()<<std::endl;

    FILE *fp = fopen(f.c_str(), "w");
    if (!fp) {
        printf("Failed to open file\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        Displacement * head = tree[i]->next;
        while(head->next){
            fprintf(fp,"%e,%e\n",head->x,head->y);
            head=head->next;
        }
    }
    fclose(fp);
    return;
}






std::vector<std::vector<Displacement*>> convertToTree(std::vector<Displacement*> nodes, int N){
    std::vector<std::vector<Displacement*>> tree;
    unsigned int size = nodes.size();
    if(nodes.empty()){
        return tree;
    }

    for(int i = 0; i<N; i++){
        std::vector<Displacement*> col;
        for(int j = 0; j<size; j+= N){
            col.push_back(nodes[i+j]);


        }
        std::reverse(col.begin(),col.end());
        tree.push_back(col);


    }

    return tree;
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

Displacement ** convertToList(std::vector<Displacement*> nodes, int N){
    unsigned int size = nodes.size();
    Displacement ** tree = (Displacement **)malloc(sizeof(Displacement*)*N+N);

    if(nodes.empty()){
        return tree;
    }

    for(int i = 0; i<N; i++){
        Displacement* node = createHead();
        for(int j = 0; j<size; j+= N){
            addToHead(node,nodes[i+j]->x,nodes[i+j]->y,nodes[i+j]->theta,nodes[i+j]->gen,nodes[i+j]->col);
        }

        *(tree+i) = node;

    }

    return tree;
}