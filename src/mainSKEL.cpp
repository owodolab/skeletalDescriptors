#include <cstdlib>
#include <iostream>
#include "time_check.hpp"
#include "graspi_types.hpp"
#include "graph_constructors.hpp"
#include "graph_io.hpp"
#include "graph_cc.hpp"
#include "performance_indicators.hpp"
#include "graspi_descriptors.hpp"
#include "graspi.hpp"
#include "skeletonization.hpp"
#include "thinning2D.hpp"
#include "skeletal_descriptors.hpp"

int main(int argc, char *argv[]) {


    if(argc==1)
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
//    if(argc>=2)
//    {
//            printf("\nargv[%d]: %s",1,argv[1]);
//    }

    const std::string inputFile = argv[1];
    const std::string outputFile = argv[2];

    int nx, ny;
    //graspi::thinning2D(inputFile,outputFile);
 //   graspi::compute_descriptors_only(vertex_colors, nx, ny, nz = 1, pixelsize, if_per);

    std::ifstream f(inputFile.c_str());
    std::string str;
    getline(f,str);
    std::istringstream iss(str);
    iss >> nx >> ny; // Reading matrix dimensions


    int **dataMatrix;
    int vertex_colors [nx*ny];
    // Allocate memory
    dataMatrix = new int*[ny];
    for(int i = 0; i < ny; i++) { // Matrix with dimensions (ny+2) * (nx+2)
        dataMatrix[i] = new int[nx];
    }

    for(int i = 0; i < ny; i++) { // Read data into matrix
        for(int j = 0; j < nx; j++) {
            f >> dataMatrix[i][j];
        }
    }
    f.close();

    graspi::dim_a_t d_a;                     //structure storing color array

    d_a.nx = nx;
    d_a.ny = ny;
    d_a.nz = 1;

    bool if_per = false;

    double pixelsize = 1.0;

    int gcount = 0;
    for(int i = 0; i < ny; i++) { // Read data into matrix
        for(int j = 0; j < nx; j++) {
            vertex_colors[gcount] = dataMatrix[i][j] ;
            gcount++;
        }
    }

    graspi::compute_skeletal_descriptors(dataMatrix, vertex_colors, nx, ny, outputFile ,pixelsize,  1, if_per);


    for(int i = 0; i < ny; i++) { // Matrix with dimensions (ny+2) * (nx+2)
       delete[] dataMatrix[i];
    }

    delete[] dataMatrix;

    return 0;
}
