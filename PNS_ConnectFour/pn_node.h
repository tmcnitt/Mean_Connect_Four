#pragma once

#include <vector>
#include "bit_board.h"
#include <memory>
#include <iostream>

extern int pn_nodes_in_exsistence;
extern bit_board board;

enum class pn_value { TRUE, FALSE, UNKNOWN };
enum class pn_type { OR, AND };

class pn_node  {
    public:
        pn_node * m_children;

        pn_type m_pn_type;
        pn_value m_pn_value;

        uint8_t m_id;

        bool m_created_children = false;
        bool m_expanded = false;

        uint32_t m_proof;
        uint32_t m_disproof;

        uint8_t m_move;

        pn_node * m_parent;

        uint8_t m_children_count = 0;

        pn_node();
        ~pn_node();

        pn_node * select_most_proving();

        void generate_all_children();
        void evaluate();
        
        pn_node * update_ancestors(pn_node *);
        
        void set_proof_and_disproof_numbers();
        void develop();

       
};


uint32_t sum_proof(pn_node * parent);
uint32_t sum_disproof(pn_node * parent);
uint32_t min_proof(pn_node * parent);
uint32_t min_disproof(pn_node * parent);
