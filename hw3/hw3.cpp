//
// Created by chase morgan on 9/18/22.
//

#include "hw3.h"
using namespace std::chrono;



void check(std::vector<Displacement*> &nodes1, std::vector<Displacement*> &nodes2, double epsilon){
    double dx1 = nodes1[1]->x-nodes1[0]->x;
    double dx2 = nodes2[1]->x-nodes2[0]->x;
    if(nodes2[0]->theta == 90){
        dx2 = 0;
    }
    double x1 = nodes1[nodes1.size()-1]->x;
    double x2 = nodes2[nodes2.size()-1]->x;
    //double nx1 = x1+(y1*sin(nodes1[0]->theta));
    //double nx2 = x2+(y2*cos(nodes2[0]->theta));

        for(int i = 0; i<nodes2.size(); i++){
            if(abs((x1+dx1) - (nodes2[i]->x)) < epsilon){
                nodes1[0]->theta = 90;
                nodes2[0]->theta = 90;
                return;
            }
        }
            if(abs((x1+dx1) - (x2+dx2)) < epsilon){
                nodes1[0]->theta = 90;
                nodes2[0]->theta = 90;

            }
    return;


}
void propagate1(std::vector<Displacement*> &nodes, double theta, double y){

    for(unsigned int i = 1; i< nodes.size(); i++){
        if(theta != 90){
            nodes[i]->x += (y*cos(theta));
        }
       // nodes[i]->x += x;
        nodes[i]->y += y;

    }
}
void propagate(std::vector<Displacement*> &nodes, double x, double y, double theta){

    for(unsigned int i = 1; i< nodes.size(); i++){

        if(theta != 90){
            nodes[i]->x += x;
        }

        nodes[i]->y += y;

    }
}


void growCol(std::vector<Displacement*> &nodes){
    //create new node and assign values then add to front of col
    Displacement *node = new Displacement();

    double y = nodes[1]->y;
    double x = nodes[1]->x-nodes[0]->x;
    double theta = nodes[0]->theta;


    node->col = nodes[0]->col;
    node->gen = nodes[0]->gen+1;
    node->y = 0;
    node->theta = theta;
    node->x = nodes[0]->x;

    nodes.insert(nodes.begin(),node);
    propagate(nodes,x,y,theta);

    return;
}



void growTree(std::vector<std::vector<Displacement*>> &tree, int generations){
    double epsilon = 5e-08;
    auto start = high_resolution_clock::now();
    size_t size = tree.size();

    for(unsigned int i = 0; i< generations; i++){

        for(unsigned int j = 0; j < size ; j++){
            if(tree[j][0]->theta < 90 && j < size-1){
               //check(tree[j], tree[j+1], epsilon);
            }
            else if( j > 0 && tree[j][0]->theta > 90) {
                //check(tree[j], tree[j - 1], epsilon);
            }
        }
        for(unsigned int j = 0; j < size ; j++) growCol(tree[j]);

    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Time taken to grow and check tree to (not parallelized)  "<<generations<<" generations: "
              << duration.count() << " milliseconds" << std::endl;
    return;
}

//*********************
//
//
//threaded functions
//
//
//
//*********************

void checkList(Displacement *head1, Displacement * head2, double epsilon) {
    double dx1 = head1->next->next->x-head1->next->x;
    double dx2 = head2->next->next->x-head2->next->x;
    double theta = head1->next->theta;
    double cx1 = head1->tail->x;
    double cx2 = head2->tail->x;
    double nx1 = head1->tail->x * sin(theta*( M_PI/180));
    double nx2 = head2->tail->x * sin(theta*( M_PI/180));
    if(head2->next->theta == 90){
        dx2 = 0;
    }

    Displacement * head = head2->next;
    while(head){
        if(abs((cx1+dx1) - (head->x)) < epsilon){
            head1->next->theta = 90;
            head2->next->theta = 90;
            return;
        }
        head=head->next;
    }
    if(abs((cx1+dx1) - (cx2+dx2)) < epsilon){
        head1->next->theta = 90;
        head2->next->theta = 90;

    }

    return;
}

void propagateList(Displacement * head, double x, double y){
    Displacement * n = head->next->next;
    double theta = n->theta;
    while(n){
//        if(n->next){
//            n->x = n->next->x;
//        }
//        else if(theta > 90){
//            n->x = n->x * sin(theta*( M_PI/180))* -1;
//        }
//        else{
//            n->x = n->x * sin(theta*( M_PI/180));
//        }
        n->x += x;

        n->y += y;
        n=n->next;
    }
}

void growColList(Displacement * head){

    double x = head->next->next->x-head->next->x;
    double y = head->next->next->y;
    addToHead(head,head->next->x,0,head->next->theta,head->next->gen+1,head->next->col);
    if(head->next->theta == 90){
        x = 0;
    }

    propagateList(head, x, y);
    return;
}

//void* checkThreaded(void *data){
//    Data_t* d = (Data_t*)data;
//    Displacement ** tree = d->tree;
//
//
//    //pthread_mutex_lock(&d->lock);
//    //int thread_part = (d->part)++;
//    //std::cout<<"part is at = "<<(d->part)<<std::endl;
//   // pthread_mutex_unlock(&d->lock);
//    unsigned int size = d->end-d->start;
//
//    //int e = d->start+(thread_part+1*(size/d->threads));
//    //int b = d->start+(thread_part*(size/d->threads));
//
//    for(unsigned int j = b ; j < e ; j++){
//        if(tree[j]->next->theta < 90 && j < d->end-1){
//            checkList(tree[j], tree[j+1], epsilon);
//        }
//        else if( j > d->start && tree[j]->next->next->theta > 90) {
//            checkList(tree[j], tree[j - 1], epsilon);
//        }
//    }
//    return NULL;
//}
//pthread_mutex_init(&d->lock, NULL);
//if(d->threads > 1){
//pthread_t threads[d->threads];
//
//for(size_t j = 0;  j < d->threads; j++){
//pthread_create(&threads[j],NULL,checkThreaded,data);
//}
//for(size_t j = 0; j< d->threads; j++){
//pthread_join(threads[j], NULL);
//}
//}
//else{
//checkThreaded(data);
//}
//(d->part) = 0;

void* growTreeBlockedList(void *data) {
    Data_t* d = (Data_t*)data;
    Displacement ** tree = d->tree;
    double epsilon = 5e-08;
    //std::cout<<d->start<<"  "<<d->end<<std::endl;

    for(unsigned int i = 0; i< d->gens; i++){
            for(unsigned int j = d->start ; j < d->end ; j++){
        if(tree[j]->next->theta < 90 && j < d->end-1){
            //checkList(tree[j], tree[j+1], epsilon);
        }
        else if( j > d->start && tree[j]->next->next->theta > 90) {
            //checkList(tree[j], tree[j - 1], epsilon);
        }
    }

        for(unsigned int j = d->start; j < d->end ; j++) growColList(tree[j]);
    }

    //std::cout<<"thread ended"<<std::endl;
    //pthread_mutex_destroy(&d->lock);
    return NULL;
}



std::vector<int> findSplitsList( Displacement ** tree, int N){
    std::vector<int> s;
    for(int i = 0; i< N-1; i++){

        if(i!= 0 && tree[i]->next->theta > 90 && tree[i+1]->next->theta < 90){

            s.push_back(i+1);
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
        chunk = floor((double) s.size() / (double) (processes));
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

