using namespace std;
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <cuda_runtime.h>
#include "helper_image.h"
#include <thrust/sort.h>
#define N 10

void medianFilterCPU(float* image, int width, int height, int windowSize, int x, int y, float* output) {

    // Skip the pixel if it is outside the image bounds
    if (x >= width || y >= height) return;

    // Create a list of the neighboring pixel values
     const int size = windowSize * windowSize;
    float* values = new float[size];
        
    int index = 0;
    for (int i = -windowSize/2; i <= windowSize/2; i++) {
        for (int j = -windowSize/2; j <= windowSize/2; j++) {
                // Compute the coordinates of the neighboring pixel
                int xn = x + i;
                int yn = y + j;
                // Skip the pixel if it is outside the image bounds
                if (xn < 0 || xn >= width || yn < 0 || yn >= height) continue;
                // Add the value of the neighboring pixel to the list
                values[index] = image[(yn * width + xn)];
                index++;
        }
    }

    // Sort the list of neighboring pixel values
    
    for (int j = 0; j < size; j++) {
        for (int k = 0; k < size - j - 1; k++) {
            if (values[k] > values[k + 1]) {
                float temp = values[k];
                values[k] = values[k + 1];
                values[k + 1] = temp;
            }
        }
    }
    output[(y * width + x)] = values[index/2];
    delete values;
}


__global__ void medianFilter(float* image, int width, int height, int windowSize, float* output) {
    // Compute the coordinates of the current pixel
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    
    //output[(y * width + x)] = y;

    // Skip the pixel if it is outside the image bounds
    if (x >= width || y >= height) return;

    // Create a list of the neighboring pixel values
     const int size = windowSize * windowSize;
    // float values[size];
    float* values = new float[size];
        
    int index = 0;
    for (int i = -windowSize/2; i <= windowSize/2; i++) {
        for (int j = -windowSize/2; j <= windowSize/2; j++) {
                // Compute the coordinates of the neighboring pixel
                int xn = x + i;
                int yn = y + j;
                // Skip the pixel if it is outside the image bounds
                if (xn < 0 || xn >= width || yn < 0 || yn >= height) continue;
                // Add the value of the neighboring pixel to the list
                values[index] = image[(yn * width + xn)];
                index++;
        }
    }
    
    //Sort the list of neighboring pixel values
    for (int j = 0; j < index; j++) {
        for (int k = 0; k < index - j - 1; k++) {
            if (values[k] > values[k + 1]) {
                float temp = values[k];
                values[k] = values[k + 1];
                values[k + 1] = temp;
            }
        }
    }
    
    output[(y * width + x)] = values[index/2];
    delete values;
}

float * gpuFilter(int filter, char* infile,char* outfile){
    printf("Start Gpu\n");
    float *imageData = NULL;
    unsigned int width, height;
    sdkLoadPGM(infile, &imageData, &width, &height);
    
    //Allocate memory for the input and output image on the device
    float* d_image;
    float* d_imageOut;
    cudaMalloc((void**)&d_image, width * height * sizeof(float));
    cudaMalloc((void**)&d_imageOut, width * height * sizeof(float));
    
    cudaThreadSetLimit(cudaLimitMallocHeapSize, 128*1024*1024);
    
        // Copy the input image from the host to the device
    cudaMemcpy(d_image, imageData, width * height  * sizeof(float), cudaMemcpyHostToDevice);
    
    //dim3 threads(16, 16);
    //dim3 blocks(512/threads.x, 512/threads.y);
    
    dim3 threads32(32,32);
    dim3 blocks32(32, 32);
    
    auto start = chrono::high_resolution_clock::now();
    medianFilter<<<blocks32, threads32>>>(d_image, width, height, filter, d_imageOut);
    //medianFilter<<<blocks, threads>>>(d_image, width, height, filter, d_imageOut);
    
    cudaDeviceSynchronize();
    
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        std::cout << "Time taken to apply median filter on GPU: "
                  << duration.count() << " milliseconds" << std::endl;

    
    
    
    
    
    
    
    cudaMemcpy(imageData, d_imageOut, width * height * sizeof(float), cudaMemcpyDeviceToHost);
    sdkSavePGM(outfile, imageData, width, height);
    cudaFree(d_image);
    return imageData;
    
}

float* cpuFilter(int filter, char* infile, char* outfile){
    printf("Start Cpu\n");
    float *imageData = NULL;
    unsigned int width, height;
    sdkLoadPGM(infile, &imageData, &width, &height);
    float *outData = new float[height*width];
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0; i < width; i++){
        for(int j = 0; j<height; j++){
            medianFilterCPU(imageData,width,height,filter,i,j,outData);
        }
    }
    auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        std::cout << "Time taken to apply median filter on CPU: "
                  << duration.count() << " milliseconds" << std::endl;
    sdkSavePGM(outfile, outData, width, height);
    return outData;
}


float cpc(float*cpu, float*gpu){
    float total_pixels = 512*512;
    // cout<<"total_pixels: "<<total_pixels<<endl;
    float not_matching_pixels = 0.0;
    for (int i = 0; i < 512; i++) {
        //cout<<"Row: "<<i<<endl;
        for (int j = 0; j < 512; j++) {
            if(gpu[i * 512 + j] != cpu[i * 512 + j]){
                not_matching_pixels += 1.0;
            }
        }
    }
    float percent_correctness = (total_pixels - not_matching_pixels) / (total_pixels) * 100;
    return percent_correctness;
}


int main(int argc, char* argv[]){
    
    int filter = atoi(argv[1]);
    float* gpu = gpuFilter(filter, argv[2],argv[3]);
    float* cpu = cpuFilter(filter,argv[2],argv[3]);
    std::cout<<cpc(cpu,gpu)<<std::endl;

}