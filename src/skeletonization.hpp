//
//  skeletonization.hpp
//  GraspiXcode
//
//  Created by Devyani Jivani on 11/17/20.
//

#ifndef skeletonization_hpp
#define skeletonization_hpp

#include<stdlib.h>
#include<stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using std::vector;

namespace graspi {

 
typedef struct{
    int row,col;
}pixel;

int** initializeSkelMatrix(int** inputMatrix, int ncol, int nrow, int initValue) {
    
    for (int i = 0; i < nrow; i++) { // Initializing with background pixels
        for (int j = 0; j < ncol; j++) {
            inputMatrix[i][j] = initValue;
        }
    }
    return inputMatrix;
}

/// This function adds a mirrored-padding to the image matrix

/// @param dataMatrix is the input morphology stored as a 2D matrix
/// @param nx is the number of voxels in x-direction
/// @param ny is the number of voxels in y-direction
/// @return the 2D skeletal matrix of the thinned morphology

int** mirrorPadding(int** dataMatrix, int p, int nx, int ny) { // padding the image with reflection of the image itself

    for(int i = 1; i < p+1; i++) { // Flip 20 pixels of matrix vertically on the top and bottom
        for(int j = p; j < nx + p; j++) {
            dataMatrix[p-i][j] = dataMatrix[i+(p-1)][j];
            dataMatrix[(ny+(p-1)) + i][j] = dataMatrix[(ny+p) - i][j];
        }
    }
    
    for(int i = 0; i < ny + (2*p); i++) { // Flip 20 pixels of matrix horizontally on both sides
        for(int j = 1; j < (p+1); j++) {
            dataMatrix[i][p - j] = dataMatrix[i][j + (p-1)];
            dataMatrix[i][(nx+(p-1)) + j] = dataMatrix[i][(nx+p) - j];
        }
    }
    return dataMatrix;
}

int getNeighbors(int** dataMatrix, int row, int col, int phasePixel){ //Number of neighbors that belong to the image

    return ((dataMatrix[row-1][col]==phasePixel) //P2
            +(dataMatrix[row-1][col+1]==phasePixel) //P3
            +(dataMatrix[row][col+1]==phasePixel) //P4
            +(dataMatrix[row+1][col+1]==phasePixel) //P5
            +(dataMatrix[row+1][col]==phasePixel) //P6
            +(dataMatrix[row+1][col-1]==phasePixel) //P7
            +(dataMatrix[row][col-1]==phasePixel) //P8
            +(dataMatrix[row-1][col-1]==phasePixel)); //P9
}
 
int getTransitions(int** dataMatrix, int row,int col, int phasePixel, int bgPixel){//Number of transitions from background pixel to phase pixel
    return(((dataMatrix[row-1][col]==bgPixel && dataMatrix[row-1][col+1]==phasePixel) //P2P3
            +(dataMatrix[row-1][col+1]==bgPixel && dataMatrix[row][col+1]==phasePixel) //P3P4
            +(dataMatrix[row][col+1]==bgPixel && dataMatrix[row+1][col+1]==phasePixel) //P4P5
            +(dataMatrix[row+1][col+1]==bgPixel && dataMatrix[row+1][col]==phasePixel) //P5P6
            +(dataMatrix[row+1][col]==bgPixel && dataMatrix[row+1][col-1]==phasePixel) //P6P7
            +(dataMatrix[row+1][col-1]==bgPixel && dataMatrix[row][col-1]==phasePixel) //P7P8
            +(dataMatrix[row][col-1]==bgPixel && dataMatrix[row-1][col-1]==phasePixel) //P8P9
            +(dataMatrix[row-1][col-1]==bgPixel && dataMatrix[row-1][col]==phasePixel))); //P9P2
}


int thinTest1(int** dataMatrix, int row, int col, int phasePixel, int bgPixel){
    int neighbours = getNeighbors(dataMatrix, row, col, phasePixel);
 
    return ((neighbours>=2 && neighbours<=6) //Condition 1
        && (getTransitions(dataMatrix, row,col, phasePixel, bgPixel)==1) //Condition 2
        && (dataMatrix[row-1][col]==bgPixel||dataMatrix[row][col+1]==bgPixel||dataMatrix[row+1][col]==bgPixel) //P2, P4, P6 Condition 3
        && (dataMatrix[row][col+1]==bgPixel||dataMatrix[row+1][col]==bgPixel||dataMatrix[row][col-1]==bgPixel)); //P4, P6, P8 Condition 4
}

int thinTest2(int** dataMatrix, int row, int col, int phasePixel, int bgPixel){
    int neighbours = getNeighbors(dataMatrix, row, col, phasePixel);
 
    return ((neighbours>=2 && neighbours<=6) //Condition 1
        && (getTransitions(dataMatrix, row,col, phasePixel, bgPixel)==1) //Condition 2
        && (dataMatrix[row-1][col]==bgPixel||dataMatrix[row][col+1]==bgPixel||dataMatrix[row][col-1]==bgPixel) // P2, P4, P8 Condition 3
        && (dataMatrix[row-1][col]==bgPixel||dataMatrix[row+1][col]==bgPixel||dataMatrix[row][col-1]==bgPixel)); // P2, P6, P8 Condition 4
}
 
/// This function computes skeletal of a  morphology defined on a structured matrix
///
/// This function computes a set of descriptors of morphology
/// @param inputdataMatrix is the input morphology stored as a 2D matrix
/// @param nx is the number of voxels in x-direction
/// @param ny is the number of voxels in y-direction
/// @return the 2D skeletal matrix of the thinned morphology

int** skeletonization2D(int** inputdataMatrix, int nx, int ny,int phasePixel, const std::string filename) {
 
    int firstny = 1, firstnx = 1, lastny, lastnx, i, j, count, endflag;
    int p = 20; //padding
 //   const int bgPixel = 0;
//    const int phasePixel = 1;
    int bgPixel = 0;
    if (phasePixel == 0)
        bgPixel = 1;
    
    int** dataMatrix;
    
    dataMatrix = new int*[ny + (2*p)];
    for(int i = 0; i < (ny + (2*p)); i++) { // Matrix with dimensions (ny+2) * (nx+2)
        dataMatrix[i] = new int[nx + (2*p)];
    }
    
    dataMatrix = initializeSkelMatrix(dataMatrix, nx + (2*p), ny + (2*p), bgPixel);
    
    for(int i = p; i < ny+p; i++){ // Assign input image matrix values to the data matrix
        for(int j = p; j < (nx+p); j++){
            dataMatrix[i][j] = inputdataMatrix[i-p][j-p];
//            std::cout << dataMatrix[i][j];
        }
//        std::cout << '\n';
    }
    
    
    dataMatrix = mirrorPadding(dataMatrix, p, nx, ny);
    
    for(int i = 0; i < ny + (2*p); i++){ // Display image matrix with padding
        for(int j = 0; j < nx + (2*p); j++){
//            std::cout << dataMatrix[i][j];
        }
//        std::cout << '\n';
    }
    
    lastny = ny + ((2*p) - 2);
    lastnx = nx + ((2*p) - 2);
//    char elapsedTimeFile[100] = "/Users/devyanijivani/Desktop/thinning2D/timeStructOrig.txt";
//    std::ofstream file;
    // performance time tab for different files
//    file.open(elapsedTimeFile, std::fstream::app);
//    file << "\n\nNew file\n\n";
//    file.close();

    count = 0;
    pixel* delpixel = new pixel [(lastny-firstny+1)*(lastnx-firstnx+1)];
    do{
    // Record start time
//        auto start = std::chrono::high_resolution_clock::now();

        int count1 = count;
 
        for(i=firstny;i<=lastny;i++){
            for(j=firstnx;j<=lastnx;j++){
                if(dataMatrix[i][j]==phasePixel && thinTest1(dataMatrix, i, j, phasePixel, bgPixel)==1){
                    delpixel[count].row = i;
                    delpixel[count].col = j;
                    count++;
                }
            }
        }
 
        endflag = (count1!=count);
 
        if(endflag){
            for(i= count1;i<count;i++){
                dataMatrix[delpixel[i].row][delpixel[i].col] = bgPixel;
        }
        }
 

        count1 = count;
        for(i=firstny;i<=lastny;i++){
            for(j=firstnx;j<=lastnx;j++){
                if(dataMatrix[i][j]==phasePixel && thinTest2(dataMatrix, i, j, phasePixel, bgPixel)==1){
                    delpixel[count].row = i;
                    delpixel[count].col = j;
                    count++;
                }
            }
        }
 
        if(endflag==0)
            endflag = (count1!=count);
 
        if(endflag){
            for(i= count1;i<count;i++){
                dataMatrix[delpixel[i].row][delpixel[i].col] = bgPixel;
        }
        }
 
        
    // Record end time
//        auto finish = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed = finish - start;
//        file.open(elapsedTimeFile, std::fstream::app);
//        file << elapsed.count() << "\n";
//        file.close();

    }while(endflag==1);

    delete[] delpixel;
    
//    for(int i = 0; i < ny + (2*p); i++){ // Display skeleton matrix with padding
//        for(int j = 0; j < nx + (2*p); j++){
//            std::cout << dataMatrix[i][j];
//        }
//        std::cout << '\n';
//    }
    
    int** skelMatrix;
    skelMatrix = new int*[ny];
    for(int i = 0; i < ny; i++) { // Matrix with dimensions (ny) * (nx)
        skelMatrix[i] = new int[nx];
    }

    skelMatrix = initializeSkelMatrix(skelMatrix, nx, ny, bgPixel);
    
    
    

    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            skelMatrix[i][j] = dataMatrix[i+p][j+p];
            
        }
//        std::cout << "\n";
    }
    

//    ofile.open(filename,std::fstream::out);
    
//    for(int i = 0; i < ny + (2*p); i++){ // Display skeleton matrix with padding
//        for(int j = 0; j < nx + (2*p); j++){
//            ofile << dataMatrix[i][j];
//        }
//        ofile<< "\n";
//    }
//    ofile.close();
    
    
//    for(int i = 0; i < (ny + (2*p)); i++) { // Matrix with dimensions (ny+2) * (nx+2)
//       delete[] dataMatrix[i];
//    }
//    delete[] dataMatrix;
    
    return skelMatrix;
}

}
#endif /* skeletonization_h */
