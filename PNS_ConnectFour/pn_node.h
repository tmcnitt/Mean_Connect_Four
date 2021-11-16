#pragma once

#include <vector>
#include "bit_board.h"
#include <memory>
#include <iostream>

extern int pn_nodes_in_exsistence;

enum class pn_value { TRUE, FALSE, UNKNOWN };
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
        
        pn_node *  m_children;
        uint32_t m_children_count = 0;

        pn_node();
        ~pn_node();

        pn_node * select_most_proving();

        void generate_all_children();
        void evaluate();
        
        pn_node * update_ancestors(pn_node *);
        
        void set_proof_and_disproof_numbers();
        void develop();

       
};