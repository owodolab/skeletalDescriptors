//
//  skeletal-descriptors.hpp
//  GraspiXcode
//
//  Created by Devyani Jivani on 2/2/21.
//

#ifndef skeletal_descriptors_hpp
#define skeletal_descriptors_hpp

namespace graspi {

    struct SKELDESC{
        std::vector<desc_t> skeldesc;
        find_desc_of_name find_desc;

        explicit SKELDESC(){}
        
        // set of methods to initiate and
                void initiate_skeletal_descriptors(){
            std::pair <float, std::string> p_desc;
            p_desc.first = -1;  p_desc.second = "SKEL_n_D";// number_of_skeletal_pixels
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_A";// number_of_skeletal_pixels
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_f_D";// average_skeletal_pixels
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_f_A";// average_skeletal_pixels
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_end_D"; // number_of_end_nodes"
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_end_A"; // number_of_end_nodes"
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_intS_D";// number_of_intersection_nodes
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_intS_A";// number_of_intersection_nodes
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_branch_D";// number_of_branch_nodes
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_branch_A";// number_of_branch_nodes
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_boundary_END_n_D";// number_of_end_nodes_on_top_and_bottom_boundary
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_boundary_END_n_A";// number_of_end_nodes_on_top_and_bottom_boundary
            skeldesc.push_back(p_desc);
            
            p_desc.first = -1;  p_desc.second = "SKEL_nB_D";// number_of_branches
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_nB_A";// number_of_branches
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_lB_D";// average_branch_length
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_lB_A";// average_branch_length
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_cycles_D";// number_of_cycles
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_cycles_A";// number_of_cycles
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_max_width_D";// maximum area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_max_width_A";// maximum area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_min_width_D";// minimum area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_min_width_A";// minimum area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_f_width_D";// average area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_f_width_A";// average area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_interface_D";// average area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_n_interface_A";// average area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_f_interface_D";// average area through network
            skeldesc.push_back(p_desc);
            p_desc.first = -1;  p_desc.second = "SKEL_f_interface_A";// average area through network
            skeldesc.push_back(p_desc);
        }
        
        void print_skeletal_descriptors(std::ostream& os, const std::string skelFile){
            std::fstream ofile;
            ofile.open(skelFile,std::fstream::out);
            for (unsigned int i=0; i< skeldesc.size();i++){
       //         if (fabs(skeldesc[i].first +1 ) > 1e-10)
                os << skeldesc[i].second << " " << skeldesc[i].first << std::endl;
                ofile << skeldesc[i].second << " " << skeldesc[i].first << std::endl;
            }
            ofile.close();
        }
        
        bool update_desc(std::string desc_name,float value){
            
            find_desc.set_desc(desc_name);
            std::vector< std::pair<float, std::string> >::iterator it;
            it = std::find_if( skeldesc.begin(), skeldesc.end(), find_desc );
            if (it == skeldesc.end())
                return false;
            else{
                (*it).first=value;
                return true;
            }
        }
        
    };// struct SKELDESC
}

#endif /* skeletal_descriptors_h */
