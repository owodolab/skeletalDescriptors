//
//  skeletal_cycles.hpp
//  GraSPI_Master
//
//  Created by Devyani Jivani on 4/16/21.
//

#ifndef skeletal_cycles_h
#define skeletal_cycles_h

#include <iostream>
#include "node_identification.hpp"


namespace graspi {

struct noEdge{
    int junctionIdx;
    int neighborIdx;
};

int getEdgeCount(int j, int edgecount,int** edges) {
    int numedge = 0 ;
    for(int i = 0; i < edgecount; i ++) {
        if((edges[i][0] == j) || (edges[i][1] == j)) {
            numedge++;
        }
    }
    return numedge;
}

int numberOfCycles(int N, int E) {

    int k = (E - N) + 1;
    // Return the number of cycles
    if(k < 0)
        k = 0;
    return k;
}

void addEdge(std::list<int> *l, int u,int v){
    l[u].push_back(v);
    l[v].push_back(u);
}

void dfsUtil(std::list<int> *l, int src,bool visited[], std::set<int>& s){
    s.insert(src);
    visited[src]=true;
    for(auto it=l[src].begin();it!=l[src].end();it++){
        if(!visited[*it]){
            dfsUtil(l, *it,visited,s);
        }
    }
}

int dfs(int v, std::list<int> *l, int edgecount, int** edges) {
    bool* visited = new bool[v];
    int c=1;
    int k = 0;
    for(int i=0;i<v;i++)
        visited[i] = false;
    for(int i=0;i<v;i++){
        if(!visited[i]){
            std::set<int>s;
            dfsUtil(l,i,visited,s);
            int numedge = 0;
            int numvert = 0;
 //           std::cout<<"Component "<< c <<" : " ;
            for(auto j=s.begin();j!=s.end();j++){
 //               std::cout<<*j<<",";
                numvert++;
                numedge = numedge + getEdgeCount(*j, edgecount, edges);
            }
            numedge = numedge/2;
 //           std::cout<<"Edge count : " << numedge << "\n";
 //           std::cout<<"Vertex count : " << numvert << "\n";
 //           std::cout<<"\n";
            k = k + numberOfCycles(numvert, numedge);
            c++;
        }
    }
    delete[] visited;
    return k;
}


int get_neighborIdx(node this_neighbor, int skelpixel, node* skeleton) {
    for(int i = 0; i < skelpixel; i++) {
        if(this_neighbor.row == skeleton[i].row && this_neighbor.col == skeleton[i].col) {
            return i;
        }
    }
    return -1;
}

noEdge isInJunction(neighbor this_neighborhood, node* branches, int n) {
    noEdge indices = {-1, -1};
    bool flag = false;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < this_neighborhood.count ; j++) {
            if(this_neighborhood.neighbors[j].row == branches[i].row && this_neighborhood.neighbors[j].col == branches[i].col) {
                indices = {i , j};
                flag  = true;
                break;
            }
        }
        if (flag == true) {
            break;
        }
    }
    return indices;
}

bool edgeExists(int x, int y, int edgecount, int** edges) {
    for(int i = 0; i < edgecount; i ++) {
        if((edges[i][0] == x && edges[i][1] == y) || (edges[i][1] == y && edges[i][0] == x)) {
            return true;
        }
    }
    return false;
}

node* getEvenNodes(neighbor skel_neighbors, int x, int skelpixel, node* skeleton, int** edges, int edgecount) {
    node* even_node = new node[4];
    even_node->count = 0 ;
    int ncount = -1;
    for(int i = 0; i < 8; i++) {
        if(skel_neighbors.direction[i] == true) {
            ncount++;
            int this_idx = get_neighborIdx(skel_neighbors.neighbors[ncount], skelpixel, skeleton);
            if (i%2 == 0 && (edgeExists(this_idx, x, edgecount, edges) == false)) {
                even_node->count++;
                even_node[even_node->count-1].row = skel_neighbors.neighbors[ncount].row;
                even_node[even_node->count-1].col = skel_neighbors.neighbors[ncount].col;
            }
        }
    }
    return even_node;
}

int getCycles(graspi::node* skeleton, int skelpixel, node* branchJunction, node* branchEnd, int phasepixel) {
    
    
    int edgecount = 0;
    
    int maxrow_edge = (skelpixel *(skelpixel - 1))/ 2;
    int** edges = new int*[maxrow_edge * 2]; // Rows

    for (int i = 0; i < maxrow_edge; i++)
    {
        edges[i] = new int[2]; // Columns
    }
    
//    for(int i = 0; i < skelpixel; i++) {
//        std::cout << i << " : " << skeleton[i].row << " , " << skeleton[i].col << "\n";
//    }
    
    
    neighbor skel_neighbors;
    node curpixel;
    for(int i = 0; i < skelpixel; i++) {
        curpixel = skeleton[i];
        skel_neighbors = getNeighbors(curpixel, skeleton, skelpixel);
        int this_idx;
 //       std::cout << branchJunction->count << "\n";
        if(isInStruct(curpixel, branchJunction, branchJunction->count) == true){
            for(int j = 0; j < skel_neighbors.count; j++) {
                this_idx = get_neighborIdx(skel_neighbors.neighbors[j], skelpixel, skeleton);
                if (edgeExists(this_idx, i, edgecount, edges) == false) {
                    edges[edgecount][0] = i;
                    edges[edgecount][1] = this_idx;
 //                   std::cout << "Junction Edge:" << edges[edgecount][0] << " , " << edges[edgecount][1] << "\n";
                    edgecount++;
                }
            }
        }
        else if(isInStruct(curpixel, branchEnd, branchEnd->count) == true) {
            this_idx = get_neighborIdx(skel_neighbors.neighbors[0], skelpixel, skeleton);
            if (edgeExists(this_idx, i, edgecount, edges) == false) {
                edges[edgecount][0] = i;
                edges[edgecount][1] = this_idx;
 //               std::cout << "End Edge:" << edges[edgecount][0] << " , " << edges[edgecount][1] << "\n";
                edgecount++;
            }
        }
        else if (skel_neighbors.count > 2){
          //      node* even_nodes = new node[4];
                node* even_nodes = getEvenNodes(skel_neighbors, i, skelpixel, skeleton, edges, edgecount);
                for (int e = 0 ; e < even_nodes->count; e++ ) {
                    this_idx = get_neighborIdx(even_nodes[e], skelpixel, skeleton);
                    edges[edgecount][0] = i;
                    edges[edgecount][1] = this_idx;
 //                   std::cout << "Even Edge:" << edges[edgecount][0] << " , " << edges[edgecount][1] << "\n";
                    edgecount++;
                }
                delete[] even_nodes;
        }
        else {
            for(int j = 0; j < skel_neighbors.count; j++) {
                this_idx = get_neighborIdx(skel_neighbors.neighbors[j], skelpixel, skeleton);
                if (edgeExists(this_idx, i, edgecount, edges) == false) {
                    edges[edgecount][0] = i;
                    edges[edgecount][1] = this_idx;
 //                   std::cout << "Branch Edge:" << edges[edgecount][0] << " , " << edges[edgecount][1] << "\n";
                    edgecount++;
                    //std::cout << "addedge!!" << "\n";
                }
            }
        }
    }
        
 //   std::cout << branchJunction->count << "\n";
 //   std::cout << branchEnd->count << "\n";
    
    
    std::list<int>* l = new std::list<int> [skelpixel];
    for (int i = 0 ; i < edgecount; i++) {
        addEdge(l, edges[i][0],edges[i][1]);
    }
    int k = 0;
    k = dfs(skelpixel, l, edgecount, edges);
    
 //   int k = 0;
 //   k = numberOfCycles(skelpixel, edgecount);
    
    for (int i = 0; i < maxrow_edge; i++)
    {
        delete[] edges[i];
    }
    
    delete[] edges;
    
    delete[] l;
    
//    std::cout << branchJunction->count << "\n";
    return k;
    }
        
} //namespace graspi
#endif /* skeletal_cycles_h */
