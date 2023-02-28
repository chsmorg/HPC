//
// Created by chase morgan on 9/18/22.
//

#include "hw2.h"
#include <math.h>
using namespace std::chrono;

void check(std::vector<Displacement*> &nodes1, std::vector<Displacement*> &nodes2, double epsilon){
    double x1 = nodes1[1]->x-nodes1[0]->x;
    double x2 = nodes2[1]->x-nodes2[0]->x;
    double cx1 = nodes1[nodes1.size()-1]->x;
    double cx2 = nodes2[nodes2.size()-1]->x;

    if(nodes1[0]->theta == 90){
        x1 = 0;
    }
    if(nodes2[0]->theta == 90){
        x2 = 0;
    }

    double dx1 = cx1+x1;
    double dx2 = cx2+x2;

    if(abs(dx1 - dx2) < epsilon || abs(cx1-cx2) < epsilon){
        nodes1[0]->theta = 90;
        nodes2[0]->theta = 90;
    }
    return;
}
void propagate(std::vector<Displacement*> &nodes, double x, double y){

    for(unsigned int i = 1; i< nodes.size(); i++){

        nodes[i]->x += x;
        nodes[i]->y += y;

    }
}

void growCol(std::vector<Displacement*> &nodes){
    //create new node and assign values then add to front of col
    Displacement *node = new Displacement();

    double x = nodes[1]->x-nodes[0]->x;
    double y = nodes[1]->y;
    node->col = nodes[0]->col;
    node->gen = nodes[0]->gen+1;
    node->y = 0;
    node->theta = nodes[0]->theta;
    node->x = nodes[0]->x;
    nodes.insert(nodes.begin(),node);
    if(nodes[0]->theta == 90){
        x = 0;
    }

    propagate(nodes, x, y);

    return;
}

void growTree(std::vector<std::vector<Displacement*>> &tree, int generations){
    double epsilon = 5e-08;
    auto start = high_resolution_clock::now();
    size_t size = tree.size();
    for(unsigned int i = 0; i< generations; i++){
        for(unsigned int j = 0; j < size ; j++){
            if(tree[j][0]->theta < 90 && j < size-1){
                check(tree[j], tree[j+1], epsilon);
            }
            else if( j < size-1 && tree[j+1][0]->theta > 90) {
                check(tree[j], tree[j + 1], epsilon);
            }
            growCol(tree[j]);


        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Time taken to grow and check tree to (not parallelized)  "<<generations<<" generations: "
              << duration.count() << " milliseconds" << std::endl;
}

//*********************
//
//
//parallelized functions
//
//
//
//*********************


void blockedGrow(Displacement * shm, int generations, std::pair<int,int> p, int N){
    for(unsigned int  i = 0; i<generations; i++) {
        for (unsigned int j = p.first; j < p.second; j++) {
            if (shm[j].next->theta < 90 && j < p.second-1) {
                checkShm(shm+j,shm+j+1);
            } else if (j < p.second-1 && shm[j + 1].next->theta > 90) {
                checkShm(shm+j,shm+j+1);
            }
            growColShm(shm,(N*7+N)+j+(N*i),shm+j);
        }
    }


}

void checkShm(Displacement * head1, Displacement * head2){
    double x1 = head1->next->next->x-head1->next->x;
    double x2 = head2->next->next->x-head2->next->x;
    double cx1 = head1->tail->x;
    double cx2 = head2->tail->x;

    if(head1->next->theta == 90){
        x1 = 0;
    }
    if(head2->next->theta == 90){
        x2 = 0;
    }

    double dx1 = cx1+x1;
    double dx2 = cx2+x2;
    double e = 5e-07;
    double t = abs(dx1-dx2);


    if(t < e){
        head1->next->theta = 90;
        head2->next->theta = 90;
    }
    return;
}

void growColShm(Displacement * shm, int index, Displacement *head){

    double x = head->next->next->x-head->next->x;
    double y = head->next->next->y;
    Displacement * node = shm+index;
    node->col = head->next->col;
    node->gen = head->next->gen+1;
    node->y = 0;
    node->theta = head->next->theta;
    node->x = head->next->x;
    node->next = head->next;
    head->next=node;
    if(head->next->theta == 90){
        x = 0;
    }

    propagateShm(head, x, y);

    return;
}

void propagateShm(Displacement * head, double x, double y){
    Displacement * n = head->next->next;
    while(n->next){
        n->x += x;
        n->y += y;
        n=n->next;
    }
}



std::vector<int> findSplitsList( Displacement ** tree, int N){
    std::vector<int> s;
    for(int i = 0; i< N-1; i++){

        if(i!= 0 && tree[i]->next->theta > 90 && tree[i+1]->next->theta < 90){

            s.push_back(i);
        }
    }
    return s;
}

std::vector<std::pair<int,int>> createBlocksList(Displacement **tree,int N, int P) {
    std::vector<int> s = findSplitsList(tree,N);

    int chunk;
    int processes = P;
    int index = 0;

    if (P > s.size()) {
        chunk = 1;
        processes = s.size();
    } else {
        if (P < 1) {
            processes = 1;
        }
        chunk = round((double) s.size() / (double) (processes));
    }
    std::vector<std::pair<int, int>> blocks;
    int i = chunk;

    while (1) {
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

