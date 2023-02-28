#include <iostream>

#include "homework_0.h"




int main(int argc, char **argv){
    FILE *fp;
    int size = atoi(argv[argc-1]);
    fp=fopen(argv[1], "r");
    std::vector<Displacement> vec1 = readDisplacementData(fp,size);
    fclose(fp);
    fp=fopen(argv[2],"r");
    std::vector<positionData> vec2 = readPositionData(fp,size);
    fclose(fp);
    fp=fopen(argv[3],"r");
    Sum t = calcStiffnessData(fp,75,75);
    fclose(fp);

    if(argc == 6){
        if(strcmp(argv[4],"true") == 0){
            std::cout<<"Output for displacement data: "<<std::endl;
            for(int i = 0; i< vec1.size(); i++){
                std::cout<<vec1[i].x<<"  "<<vec1[i].y<<"  "<<vec1[i].theta<<std::endl;
            }
            std::cout<<"\nOutput for position data: "<<std::endl;
            for(int i = 0; i< vec2.size(); i++){
                std::cout<<vec2[i].x<<"  "<<vec2[i].y<<"  "<<vec2[i].row<<"  "<<vec2[i].col<<std::endl;
            }
            std::cout<<"\nOutput for matrix data: "<<std::endl;
            for(int i = 0; i< 75; i++){
                std::cout<<"sum for row: "<<i<<" =  "<<t.rowsSum[i]<<std::endl;
            }
            for(int i = 0; i< 75; i++){
                std::cout<<"sum for col: "<<i<<" =  "<<t.colsSum[i]<<std::endl;
            }
            std::cout<<"sum for total =  "<<t.total<<std::endl;

        }
    }

















}