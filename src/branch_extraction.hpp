//
//  branch_extraction.hpp
//  GraspiXcode
//
//  Created by Devyani Jivani on 3/15/21.
//

#ifndef branch_extraction_hpp
#define branch_extraction_hpp

#include "node_identification.hpp"

namespace graspi {

typedef struct{
    int num_of_branches;
    double average_branch_length;
}branch;

/// This function counts the number of times a pixel is in a structure

/// @param thispixel is the pointer to the pixel
/// @param branches is the pointer to the structure
/// @param n is the number of pixels in branches
/// @return count of occurence of thispixel in branches

int isInCount(node thispixel, node* branches, int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
        if(thispixel.row == branches[i].row && thispixel.col == branches[i].col) {
            count++;
        }
    }
    return count;
}

/// This function returns the untraversed pixel according to the priority (refer document)

/// @param this_neighbor is the pointer to the neighbors of a pixel just traversed
/// @param branches is the pointer to the structure of all branches stored
/// @param n is the number of pixels in branches
/// @return the next node to be traversed

node getUntraversed(neighbor this_neighbor,  node branches[], int n) {
   // node* priority = new node[89];
    node temp_node;
    temp_node.row = -1;
    temp_node.col = -1;
    temp_node.count = 0;
    int ncount = -1;
    for(int i = 0; i < 8; i++) {
        if(this_neighbor.direction[i] == true) {
            ncount++;
            if (i%2 == 0 && (isInStruct(this_neighbor.neighbors[ncount], branches, n) == false))
                return this_neighbor.neighbors[ncount];
        }
    }
    ncount = -1;
    for(int i = 0; i < 8; i++) {
        if(this_neighbor.direction[i] == true) {
            ncount++;
            if (isInStruct(this_neighbor.neighbors[ncount], branches, n) == false)
                return this_neighbor.neighbors[ncount];
        }
    }
    
    return temp_node;
    
}

/// This function returns the untraversed pixel according to the priority (refer document)

/// @param skeleton is the pointer to the skeleton
/// @param skelpixel is the size of the skeleton
/// @param branchJunction is the pointer to the intersections in the skeleton
/// @param branchEnd is the pointer to the ends of the skeleton
/// @param phasepixel is the value corresponding to the phase
/// @return descriptors

branch* getBranches(graspi::node *skeleton, int skelpixel, node* branchJunction, node* branchEnd, int phasepixel) {
    
//    std::cout << "I am in branch extraction\n";
    
    node* branches = new node[2*skelpixel];
 //   node* branches = new node[skelpixel + branchJunction->count + branchEnd->count];
    int bcount = 0;
    node curpixel;
    neighbor first_neighbors;
 //   std::cout << branchJunction->count << "\n";
    for(int p = 0; p < branchJunction->count; p++) {
        curpixel = branchJunction[p];
 //       std::cout << branchJunction->count << "\n";
        first_neighbors = getNeighbors(curpixel, skeleton, skelpixel);
        if(isInCount(curpixel, branches, bcount) == first_neighbors.count) {
            continue;
        }
        else {
            
            branches[bcount].row = curpixel.row;
            branches[bcount].col = curpixel.col;
            bcount++;
            
            
            
 //           curpixel = getUntraversed(first_neighbors, branches, bcount); //check for even neighbors
            for (int n = 0; n < first_neighbors.count; n++) {
                
                curpixel = getUntraversed(first_neighbors, branches, bcount);
                if (curpixel.row == -1) {
                    continue;
                }
                if(isInStruct(curpixel, branches, bcount) == true) {
                    continue;
                }
                else {
                    bool flag = true;
                    while(flag == true){
                        if((isInStruct(curpixel, branchJunction, branchJunction->count) == true) || (isInStruct(curpixel, branchEnd, branchEnd->count) == true)) {
   //                         std::cout << branchJunction->count << "\n";
                            branches[bcount].row = curpixel.row;
                            branches[bcount].col = curpixel.col;
                            bcount++;
                            branches[bcount].row = -2;
                            branches[bcount].col = -2;
                            bcount++;
                            break;
                        }
                        else {
                            branches[bcount].row = curpixel.row;
                            branches[bcount].col = curpixel.col;
                            bcount++;
                        }
                        neighbor next_local_neighbor;
                        next_local_neighbor = getNeighbors(curpixel, skeleton, skelpixel);
                        node priority_node;
                        if (next_local_neighbor.count > 2) {
                            priority_node = getUntraversed(next_local_neighbor, branches, bcount);
                            if (priority_node.row == -1) {
                                break;
                            }
                            if((isInStruct(priority_node, branchJunction, branchJunction->count) == true) || (isInStruct(priority_node, branchEnd, branchEnd->count) == true)) {
  //                              std::cout << branchJunction->count << "\n";
                                branches[bcount].row = priority_node.row;
                                branches[bcount].col = priority_node.col;
                                bcount++;
                                branches[bcount].row = -2;
                                branches[bcount].col = -2;
                                bcount++;
                                flag = false;
                            }
                            else {
                                branches[bcount].row = priority_node.row;
                                branches[bcount].col = priority_node.col;
                                bcount++;
                                next_local_neighbor = getNeighbors(priority_node, skeleton, skelpixel);
                                curpixel = getUntraversed(next_local_neighbor, branches, bcount);
                                if (curpixel.row == -1) {
                                    continue;
                                }
                            }
                        }
                        else {
                            priority_node = getUntraversed(next_local_neighbor, branches, bcount);
                            if (priority_node.row == -1) {
                                break;
                            }
                            if((isInStruct(priority_node, branchJunction, branchJunction->count) == true) || (isInStruct(priority_node, branchEnd, branchEnd->count) == true)) {
  //                              std::cout << branchJunction->count << "\n";
                                branches[bcount].row = priority_node.row;
                                branches[bcount].col = priority_node.col;
                                bcount++;
                                branches[bcount].row = -2;
                                branches[bcount].col = -2;
                                bcount++;
                                flag = false;
                            }
                            else {
                                branches[bcount].row = priority_node.row;
                                branches[bcount].col = priority_node.col;
                                bcount++;
                                next_local_neighbor = getNeighbors(priority_node, skeleton, skelpixel);
                                curpixel = getUntraversed(next_local_neighbor, branches, bcount);
                            }
                        }
                    }
                }
            }
        }
    }
//    std::cout << branchJunction->count << "\n";
//    delete curpixel;
//    delete first_neighbors;
    
    for (int e = 0; e < branchEnd->count; e++) {
 //       node* curpixel = new node;
        curpixel = branchEnd[e];
        if(isInStruct(curpixel, branches, bcount) == true) {
            continue;
        }
//        neighbor* first_neighbors = new neighbor;
        first_neighbors = getNeighbors(curpixel, skeleton, skelpixel);
        branches[bcount].row = curpixel.row;
        branches[bcount].col = curpixel.col;
        bcount++;
        if(first_neighbors.count > 0) {
            curpixel = getUntraversed(first_neighbors, branches, bcount);
            if (curpixel.row == -1) {
                continue;
            }
            bool flag = true;
            while(flag == true){
                if((isInStruct(curpixel, branchEnd, branchEnd->count) == true)) {
                    branches[bcount].row = curpixel.row;
                    branches[bcount].col = curpixel.col;
                    bcount++;
                    branches[bcount].row = -2;
                    branches[bcount].col = -2;
                    bcount++;
                    flag = false;
                }
                else if(curpixel.row == -1) {
                    branches[bcount].row = -2;
                    branches[bcount].col = -2;
                    bcount++;
                    flag = false;
                }
                else {
                    branches[bcount].row = curpixel.row;
                    branches[bcount].col = curpixel.col;
                    bcount++;
                    neighbor next_local_neighbor;
                    next_local_neighbor = getNeighbors(curpixel, skeleton, skelpixel);
                    curpixel = getUntraversed(next_local_neighbor, branches, bcount);
                }
            }
        }
    }
    
//    delete curpixel;
//    delete first_neighbors;
    
//    node* curpixel = new node;
//    neighbor* first_neighbors = new neighbor;
    for (int i = 0; i < skelpixel; i++) {
        curpixel = skeleton[i];
        if(isInStruct(curpixel, branches, bcount) == true) {
            continue;
        }
        
        first_neighbors = getNeighbors(curpixel, skeleton, skelpixel);
        branches[bcount].row = curpixel.row;
        branches[bcount].col = curpixel.col;
        bcount++;
        if(first_neighbors.count > 0) {
            curpixel = getUntraversed(first_neighbors, branches, bcount);
            if (curpixel.row == -1) {
                continue;
            }
            // CHANGE CONDITIONS
            bool flag = true;
            neighbor next_local_neighbor;
            while(flag == true) {
                if((isInStruct(curpixel, branchEnd, branchEnd->count) == true)) {
                    branches[bcount].row = curpixel.row;
                    branches[bcount].col = curpixel.col;
                    bcount++;
                    branches[bcount].row = -2;
                    branches[bcount].col = -2;
                    bcount++;
                    flag = false;
                }
                else if(curpixel.row == -1) {
                    branches[bcount].row = -2;
                    branches[bcount].col = -2;
                    bcount++;
                    flag = false;
                }
                else {
                    branches[bcount].row = curpixel.row;
                    branches[bcount].col = curpixel.col;
                    bcount++;
                    next_local_neighbor = getNeighbors(curpixel, skeleton, skelpixel);
                    curpixel = getUntraversed(next_local_neighbor, branches, bcount);
                }
            }
        }
    }
    

    int branch_count = 0;
    float sum_branch_len = 0.0;
    int this_branchlength = 0;
    for(int i = 0; i < bcount; i++) {
//        std::cout << branches[i].row << " , " << branches[i].col << "\n";
        this_branchlength++;
        if (branches[i].row == -2) {
            branch_count++;
 //           std::cout << this_branchlength << "\n";
            sum_branch_len += this_branchlength - 1;
 //           std::cout << sum_branch_len << "\n";
            this_branchlength = 0;
        }
    }
    
 //   std::cout << sum_branch_len/branch_count << "\n";
 //   delete[] branches;
    
    
    branch* this_branch = new branch;
    this_branch->num_of_branches = branch_count;
    if(branch_count <= 0){
        this_branch->average_branch_length = 0;
    }
    else {
        this_branch->average_branch_length = sum_branch_len/branch_count;
    }
    
//    std::cout<< "Branches : " << branch_count << "\n";
//    std::cout << branchJunction->count << "\n";
    return this_branch;




}

}
#endif /* branch_extraction_h */
