//
//  thinning2D.hpp
//  GraspiXcode
//
//  Created by Devyani Jivani on 2/2/21.
//

#ifndef thinning2D_hpp
#define thinning2D_hpp

#include "graspi_types.hpp"
#include "graph_constructors.hpp"
#include "performance_indicators.hpp"
#include "graph_cc.hpp"
#include "skeletal_descriptors.hpp"
#include "skeletonization.hpp"
#include "node_identification.hpp"
#include "branch_extraction.hpp"
#include "graspi.hpp"
#include "skeletal_cycles.hpp"
#include "interfacial_descriptors.hpp"
#include <cmath>


namespace graspi {


// std::vector<std::pair<float,std::string> > compute_descriptors(const std:vector<unsigned int>& vertex_colors){

int matToIndex(int C, int row, int col) {
    return (C * row) + col;
}


std::vector<desc_t> compute_skeletal_descriptors(int **dataMatrix, int* p_colors, int nx, int ny, const std::string outputFile, double pixelsize, int phasePixel, const bool& if_per = true){

    size_t lastindex = outputFile.find_last_of(".");
    std::string rawname = outputFile.substr(0, lastindex);

    // Average Distance to the interface
    int nz = 0;
    // this code is valid only for n_phases = 2;
    int sizeColors=nx*ny*nz+3;
    if (nz ==0) sizeColors = nx*ny+3;

    graspi::vertex_colors_t vertex_colors;
    vertex_colors.resize(sizeColors);


    for (int i = 0; i < sizeColors; i++) {
        vertex_colors[i]=p_colors[i];
  //      std::cout << vertex_colors[i];
    }

    vertex_colors[sizeColors] = 0;
    vertex_colors[sizeColors + 1] = 0;
    vertex_colors[sizeColors + 2] = 0;
    //        vertex_colors.assign(p_colors, p_colors + sizeColors);

    // these are settings for two phase morphology
    int n_of_phases=2;
    unsigned int n_bulk = nx*ny*1;
    unsigned int n_meta = 2;

    graspi::graph_t*        G;
    graspi::dim_g_t         d_g(n_of_phases,n_bulk,n_meta); //structure storing basic dimensions of G
    graspi::dim_a_t         d_a(nx,ny,nz);            //structure storing color array dimensions
    //    graspi::vertex_colors_t vertex_colors;   //container storing colors of vertices
    graspi::vertex_ccs_t    vertex_ccs;      //container storing CC-indices of vertices
    graspi::edge_colors_t   edge_colors;     //container storing colors of edges(f,s,t)
    graspi::edge_map_t      m;
    graspi::edge_weights_t  edge_weights(m); //container storing edge weights
    graspi::ccs_t           ccs;            //container storing basic info of CCs
    graspi::DESC          descriptors;    //container (vector) storing all descriptors


    /***********************************************************************
     * Graph construction                                                  *
     *                                                                     *
     **********************************************************************/

    if( ! graspi::build_graph(G, d_g,
                              vertex_colors, d_a,
                              edge_weights, edge_colors,
                              pixelsize, if_per) ){
        std::cout << "Problem with building graph! " << std::endl;
        //        break;
    }

    graspi::identify_connected_components( G, vertex_colors, vertex_ccs );
    graspi::determine_basic_stats_of_ccs( G, d_g, ccs,
                                         vertex_colors, vertex_ccs);

    /***********************************************************************
     * Performance Indicators Computations                                 *
     *                                                                     *
     *                                                                     *
     **********************************************************************/

//    all_perfomance_indicators_2phases( descriptors,
  //                                    G, d_g,
 //                                     vertex_colors, d_a, edge_weights, edge_colors,
  //                                    vertex_ccs, ccs,
//                                      pixelsize);
 //

//    std::pair<double,double> F_DISS = wf_diss(G,d_g,vertex_colors,edge_weights,vertex_ccs,ccs,Ld,BLACK,GREEN);
    COLOR bcolor = WHITE;
    COLOR green = GREEN;
    COLOR wcolor = BLACK;


    int **skelImageMatrix;
    phasePixel = 1;
    int bgPixel = 0;


    skelImageMatrix = new int*[ny];
    for(int i = 0; i < (ny); i++) { // Matrix to store the skeleton (ny * nx)
        skelImageMatrix[i] = new int[nx];
    }

    skelImageMatrix = initializeSkelMatrix(skelImageMatrix, nx, ny, bgPixel);

    float thisphase = 0.0;
    for(int i=0;i<ny;i++) {
        for(int j=0;j<nx;j++) {
            if(dataMatrix[i][j]==phasePixel) {
                thisphase++;
            }
 //           std::cout << dataMatrix[i][j] << " ";
        }
//        std::cout <<  "\n";
    }
//    std::cout <<  "\n\n\n";
    std::string skelfilename;
    skelfilename = rawname + std::string("_skeleton_D.txt");
    skelImageMatrix = skeletonization2D(dataMatrix, nx, ny, phasePixel, skelfilename);

    SKELDESC          skeletal_descriptors;    //container (vector) storing all skeletal descriptors

    skeletal_descriptors.initiate_skeletal_descriptors();

    node* skeleton_D = new node[(ny+1)*(nx+1)];

    int skelpixel_D = 0;
    for(int i=0;i<ny;i++) {
        for(int j=0;j<nx;j++) {
            if(skelImageMatrix[i][j]==phasePixel) {
                skeleton_D[skelpixel_D].row = i;
                skeleton_D[skelpixel_D].col = j;
                skelpixel_D++;
            }
//            std::cout << skelImageMatrix[i][j] << " ";
        }
//        std::cout <<  "\n";
    }

    skeletal_descriptors.update_desc("SKEL_n_D",skelpixel_D);
    skeletal_descriptors.update_desc("SKEL_f_D", (skelpixel_D/thisphase));

    int interface_count = interfaceArea(dataMatrix, nx, ny, phasePixel);

    skeletal_descriptors.update_desc("SKEL_n_interface_D", interface_count);

    skeletal_descriptors.update_desc("SKEL_f_interface_D", (interface_count/thisphase));
    // Node Identification

 //   node* branchJunction = new node[nx*ny];
 //   node* branchEnd = new node[nx*ny];
    node* branchEnd_D = new node [skelpixel_D];
    node* branchJunction_D = new node [skelpixel_D];
    identifyEnds(skeleton_D, skelpixel_D, 1, branchEnd_D);

    identifyIntersections(skeleton_D, skelpixel_D, branchEnd_D, 1, branchJunction_D);


//    for(int i = 0; i < branchEnd_D->count; i++) {
//        std::cout << branchEnd_D[i].row << " , " << branchEnd_D[i].col << "\n";
//    }


//    for(int i = 0; i < branchJunction->count; i++) {
//        std::cout << branchJunction[i].row << " , " << branchJunction[i].col;
//    }


    int ends_on_boundary = 0;
    for(int i = 0; i < branchEnd_D->count; i++) {
 //       std::cout << branchEnd_D[i].row << " , " << branchEnd_D[i].col << "\n";
        if(branchEnd_D[i].row == ny || branchEnd_D[i].row == ny - 1 || branchEnd_D[i].row == 0 || branchEnd_D[i].row == 1 ) {
            ends_on_boundary++;
 //           std::cout << branchEnd_D[i].row << " , " << branchEnd_D[i].col << "\n";
        }
    }

    skeletal_descriptors.update_desc("SKEL_n_end_D", branchEnd_D->count);
    skeletal_descriptors.update_desc("SKEL_n_intS_D", branchJunction_D->count);
    skeletal_descriptors.update_desc("SKEL_n_branch_D", skelpixel_D - (branchEnd_D->count + branchJunction_D->count));

    skeletal_descriptors.update_desc("SKEL_boundary_END_n_D", ends_on_boundary);

    branch* branch_info_D = getBranches(skeleton_D, skelpixel_D, branchJunction_D, branchEnd_D, phasePixel);

    skeletal_descriptors.update_desc("SKEL_nB_D", branch_info_D->num_of_branches);
    skeletal_descriptors.update_desc("SKEL_lB_D", branch_info_D->average_branch_length);


    int cycles_D = getCycles(skeleton_D, skelpixel_D, branchJunction_D, branchEnd_D, phasePixel);
    skeletal_descriptors.update_desc("SKEL_n_cycles_D", cycles_D);

    connect_color_green bpred(*G,vertex_colors,bcolor,green);
    unsigned int bn = boost::num_vertices(*G);
    vertex_t bint_id = d_g.id(green);
    std::vector<float> db(bn);
//    std::vector<float> d_white(wn);
    determine_shortest_distances( G, edge_weights, bint_id, bpred, db);

    int b_color = 0;
    double sum_black = 0;
    std::vector<float> bs_distance(skelpixel_D);
    float minb = bn;
    float maxb = 0;
//    std::vector<float> bs_distance(b_color);
    int id;
    int skelcount = 0;
    std::ostringstream oss_out_d;

    for(int i = 0; i < skelpixel_D; i++){
        id = matToIndex(nx, skeleton_D[i].row, skeleton_D[i].col);
        unsigned int c = vertex_colors[id];
        if (c == bcolor) {
            b_color++;
            if (fabs(db[id]) < nx*ny && db[id] > 0) {
                bs_distance[skelcount] =  roundf((db[id]) * 100) / 100;
                double d_i = bs_distance[skelcount];
                oss_out_d  << d_i       << std::endl;
//                std::cout << ws_distance[skelcount] << "\n";
                if(bs_distance[skelcount] > maxb)
                    maxb = bs_distance[skelcount];
                if(bs_distance[skelcount] < minb)
                    minb = bs_distance[skelcount];
                skelcount++;
                sum_black = sum_black + bs_distance[skelcount-1];
            }
        }
    }
//    std::cout << b_color << "\n";
//    std::string filename;
//    filename = rawname + std::string("_DistancesToInterface_D.txt");
//    std::ofstream f_out(filename.c_str());
//    std::string buffer = oss_out_d.str();
//    int size = oss_out_d.str().size();
//    f_out.write (buffer.c_str(),size);
//    f_out.close();

//    boost::centrality_map centrality;
//    brandes_betweenness_centrality(G);
    skeletal_descriptors.update_desc("SKEL_max_width_D", maxb);
    skeletal_descriptors.update_desc("SKEL_min_width_D", minb);
    skeletal_descriptors.update_desc("SKEL_f_width_D", sum_black/b_color);

    delete branch_info_D;
    delete [] branchEnd_D;
    delete [] branchJunction_D;

    delete[] skeleton_D;

//    for(int i = 0; i < ny; i++) { // Matrix with dimensions (ny+2) * (nx+2)
//       delete[] skelImageMatrix[i];
//    }
//    delete[] skelImageMatrix;



    phasePixel = 0;
    bgPixel = 1;

 //   skelImageMatrix = initializeSkelMatrix(skelImageMatrix, nx, ny, bgPixel);

    thisphase = 0.0;
    for(int i=0;i<ny;i++) {
        for(int j=0;j<nx;j++) {
            if(dataMatrix[i][j]==phasePixel) {
                thisphase++;
            }
 //           std::cout << dataMatrix[i][j] << " ";
        }
//        std::cout <<  "\n";
    }
 //       std::cout <<  "\n\n\n";
        skelfilename = rawname + std::string("_skeleton_A.txt");
        skelImageMatrix = skeletonization2D(dataMatrix, nx, ny, phasePixel, skelfilename);

        node* skeleton_A = new node[(ny+1)*(nx+1)];

        int skelpixel_A = 0;
        for(int i=0;i<ny;i++) {
            for(int j=0;j<nx;j++) {
                if(skelImageMatrix[i][j]==phasePixel) {
                    skeleton_A[skelpixel_A].row = i;
                    skeleton_A[skelpixel_A].col = j;
                    skelpixel_A++;
                }
 //               std::cout << skelImageMatrix[i][j] << " ";
            }
 //           std::cout <<  "\n";
        }

    skeletal_descriptors.update_desc("SKEL_n_A",skelpixel_A);
    skeletal_descriptors.update_desc("SKEL_f_A", (skelpixel_A/thisphase));

    interface_count = interfaceArea(dataMatrix, nx, ny, phasePixel);

    skeletal_descriptors.update_desc("SKEL_n_interface_A", interface_count);

    skeletal_descriptors.update_desc("SKEL_f_interface_A", (interface_count/thisphase));

    node* branchEnd_A = new node [skelpixel_A];
    node* branchJunction_A = new node [skelpixel_A];
    identifyEnds(skeleton_A, skelpixel_A, phasePixel, branchEnd_A);

    identifyIntersections(skeleton_A, skelpixel_A, branchEnd_A, phasePixel, branchJunction_A);

    ends_on_boundary = 0;
    for(int i = 0; i < branchEnd_A->count; i++) {
        if(branchEnd_A[i].row == ny || branchEnd_A[i].row == ny - 1 || branchEnd_A[i].row == 0 || branchEnd_A[i].row == 1 ) ends_on_boundary++;
    }

    skeletal_descriptors.update_desc("SKEL_n_end_A", branchEnd_A->count);
    skeletal_descriptors.update_desc("SKEL_n_intS_A", branchJunction_A->count);
    skeletal_descriptors.update_desc("SKEL_n_branch_A", skelpixel_A - (branchEnd_A->count + branchJunction_A->count));

    skeletal_descriptors.update_desc("SKEL_boundary_END_n_A", ends_on_boundary);


 //   std::cout << branchJunction_A->count << "\n";
    branch* branch_info_A = getBranches(skeleton_A, skelpixel_A, branchJunction_A, branchEnd_A, phasePixel);

 //   std::cout << branchJunction_A->count << "\n";
    skeletal_descriptors.update_desc("SKEL_nB_A", branch_info_A->num_of_branches);
    skeletal_descriptors.update_desc("SKEL_lB_A", branch_info_A->average_branch_length);

    delete branch_info_A;

    int cycles_A = getCycles(skeleton_A, skelpixel_A, branchJunction_A, branchEnd_A, phasePixel);
    skeletal_descriptors.update_desc("SKEL_n_cycles_A", cycles_A);

    connect_color_green wpred(*G, vertex_colors, wcolor, green);
    unsigned int wn = boost::num_vertices(*G);
    vertex_t wint_id = d_g.id(green);
    std::vector<float> dw(wn);
//    std::vector<float> d_white(wn);
    determine_shortest_distances( G, edge_weights, wint_id, wpred, dw);

    int w_color = 0;
    double sum_white = 0;
    std::vector<float> ws_distance(skelpixel_A);
    float minw = wn;
    float maxw = 0;
//    std::vector<float> bs_distance(b_color);

    int idx;
    skelcount = 0;
    std::ostringstream oss_out_dw;

    for(int i = 0; i < skelpixel_A; i++){
        idx = matToIndex(nx, skeleton_A[i].row, skeleton_A[i].col);
        unsigned int c = vertex_colors[idx];
        if (c == wcolor) {
            w_color++;
            if (fabs(dw[idx]) < nx*ny && dw[idx] > 0) {
                ws_distance[skelcount] =  roundf((dw[idx]) * 100) / 100;
                double d_i = ws_distance[skelcount];
                oss_out_dw  << d_i << std::endl;
//                std::cout << ws_distance[skelcount] << "\n";
                if(ws_distance[skelcount] > maxw)
                    maxw = ws_distance[skelcount];
                if(ws_distance[skelcount] < minw)
                    minw = ws_distance[skelcount];
                skelcount++;
                sum_white += ws_distance[skelcount-1];
            }
        }
    }

//    std::cout << w_color << "\n";

//    filename = rawname + std::string("_DistancesToInterface_A.txt");
//    f_out.open(filename.c_str());
//    std::string bufferw = oss_out_dw.str();
//    int sizew = oss_out_dw.str().size();
//    f_out.write (bufferw.c_str(),sizew);
//    f_out.close();

    skeletal_descriptors.update_desc("SKEL_max_width_A", maxw);
    skeletal_descriptors.update_desc("SKEL_min_width_A", minw);
    skeletal_descriptors.update_desc("SKEL_f_width_A", sum_white/w_color);

    delete [] branchEnd_A;
    delete [] branchJunction_A;

    for(int i = 0; i < ny; i++) { // Matrix with dimensions (ny+2) * (nx+2)
       delete[] skelImageMatrix[i];
    }
    delete[] skelImageMatrix;

    delete[] skeleton_A;






    skeletal_descriptors.print_skeletal_descriptors(std::cout, outputFile);






    return skeletal_descriptors.skeldesc;
}

}

#endif /* thinning2D_h */
