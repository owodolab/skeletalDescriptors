//
//  interfacial_descriptors.hpp
//  GraSPI_Master
//
//  Created by Devyani Jivani on 6/16/21.
//

#ifndef interfacial_descriptors_h
#define interfacial_descriptors_h

#include<stdlib.h>
#include<stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "skeletonization.hpp"


int interfaceArea(int **imageMatrix, int nx, int ny, int phasePixel) {
    int neighbor_count = 0;
    int interface = 0;
    for(int i=1;i<ny-1;i++) {
        for(int j=1;j<nx-1;j++) {
            if(imageMatrix[i][j]==phasePixel) {
                neighbor_count = graspi::getNeighbors(imageMatrix, i, j, phasePixel);
                if(neighbor_count < 8)
                    interface++;
            }
 //           std::cout << dataMatrix[i][j] << " ";
        }
//        std::cout <<  "\n";
    }
    return interface;
}

#endif /* interfacial_descriptors_h */
