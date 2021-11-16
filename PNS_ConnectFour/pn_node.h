#pragma once
#include <vector>
#include "bit_board.h"
#include <memory>

enum class pn_value { TRUE , FALSE, UNKNOWN };


enum class pn_type { OR, AND };

class pn_node  {
    public:
        uint32_t m_id;

        pn_type m_pn_type;
        pn_value m_pn_value;

        uint32_t m_proof;
        uint32_t m_disproof;

        bit_board m_board;

        pn_node * m_parent;
        
        std::vector<pn_node*> m_children;

        pn_node * select_most_proving();

        void generate_all_children();
        void evaluate();
        
        void update_ancestors();
        
        void set_proof_and_disproof_numbers();
        void develop();

        //~pn_node(){
        //    for(int i = 0; i < this->m_children.size(); i++){
        //        delete this->m_children[i];
        //    }
        //}
};