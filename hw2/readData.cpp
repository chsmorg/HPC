//
// Created by chase morgan on 9/18/22.
//
#include <cmath>
#include "hw2.h"

void writeTreeToCsv(std::vector<std::vector<Displacement*>> tree){
    FILE *fp = fopen("displacements.csv","w");
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

void writeTreeToCsvShm(Displacement * tree, int N) {
    FILE *fp = fopen("displacements.csv", "w");
    if (!fp) {
        printf("Failed to open file\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        Displacement * head = (tree+i)->next;
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

void storeInShm(Displacement** tree, int N, Displacement * shm){
    std::vector<Displacement*> tv;
    for(int i = 0; i<N; i++){
        Displacement * head = tree[i];
        shm[i].theta = tree[i]->theta;
        shm[i].x = tree[i]->x;
        shm[i].y = tree[i]->y;
        shm[i].gen = tree[i]->gen;
        shm[i].col = tree[i]->col;
        Displacement * shmHead = shm+i;
        Displacement * shmNext = shmHead;
        int j = 1;
        while(head->next){
            head = head->next;
            shm[i+(N*j)].theta = head->theta;
            shm[i+(N*j)].x = head->x;
            shm[i+(N*j)].y = head->y;
            shm[i+(N*j)].gen = head->gen;
            shm[i+(N*j)].col = head->col;
            shmNext->next = &shm[i+(N*j)];
            shmNext = shmNext->next;
            j++;
        }
        shmHead->tail = shmNext;
    }

}








//unused functions
//
//
//
//

std::vector<std::vector<std::vector<Displacement*>>> createBlocks(std::vector<std::vector<Displacement*>> tree, std::vector<int> s, int P){
int chunk;
int processes = P;
int index = 0;

if(P>s.size()){
chunk = 1;
processes = s.size();
}
else{
if(P<1){
processes = 1;
}
chunk = round((double)s.size()/(double)(processes));
}
std::vector<std::vector<std::vector<Displacement*>>> blocks;
int i = chunk;

while(1){
if(processes> 1){
std::vector<std::vector<Displacement*>> b;
for(; index <= s[i-1]; index++){
b.push_back(tree.at(index));
}
blocks.push_back(b);
i += chunk;
processes--;
}
else{
std::vector<std::vector<Displacement*>> b;
for(; index<tree.size(); index++){
b.push_back(tree.at(index));
}
blocks.push_back(b);
break;
}


}

return blocks;

}

std::vector<int> findSplits(std::vector<std::vector<Displacement*>> tree, int N){
    std::vector<int> s;
    for(int i = 0; i< N-1; i++){

        if(i!= 0 && tree[i][1]->theta > 90 && tree[i+1][1]->theta < 90){

            s.push_back(i);
        }
    }
    return s;
}
