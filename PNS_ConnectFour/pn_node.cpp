#include <vector>
#include "bit_board.h"
#include "pn_node.h"
#include <algorithm>
#include <iostream>
#include <cassert>

int pn_nodes_in_exsistence = 0;

pn_node::pn_node(){
    pn_nodes_in_exsistence += 1;
}

pn_node::~pn_node(){
    pn_nodes_in_exsistence -= 1;

    if(this->m_children_count > 0){
        delete [] this->m_children;
    }
}

pn_node * pn_node::select_most_proving() {
    pn_node * node = this;

    while (node->m_children_count > 0) {
        int i = 0;

        if (node->m_pn_type == pn_type::OR) {
            while (node->m_children[i].m_proof != node->m_proof) {
                i += 1;
            }
        } else {
            while (node->m_children[i].m_disproof != node->m_disproof) {
                i += 1;
            }
        }

        //assert(i < node->m_children.size());

        node = &node->m_children[i];
    }

    return node;
}

void pn_node::evaluate(){
    this->m_pn_value = pn_value::UNKNOWN;

    if(this->m_board.haswon(0)){
       //std::cout << "Found a way player 1 wins" << std::endl;
        this->m_pn_value = pn_value::TRUE;
    } else if(this->m_board.haswon(1)){
        //std::cout << "Found a way player 1 wins" << std::endl;
        this->m_pn_value = pn_value::FALSE;
    } else if(this->m_board.isfull()) {
        this->m_pn_value = pn_value::FALSE;
    }
}

void pn_node::generate_all_children(){
    int player = 1;
    pn_type type = pn_type::OR;

    if(this->m_pn_type == type){
        type = pn_type::AND;
        player = 0;
    }

    if(this->m_pn_value != pn_value::UNKNOWN){
        return;
    }

    assert(this->m_pn_value == pn_value::UNKNOWN);

    this->m_children = new pn_node[WIDTH];

    for(int col = 0; col < WIDTH; col++){
        if(this->m_board.isplayable(col)){
            bit_board child_board(this->m_board);
            child_board.makemove(player, col);

            pn_node child = pn_node();
            child.m_id = this->m_id + 1;
            child.m_parent = this;
            child.m_board = child_board;
            child.m_pn_type = type;
            child.m_pn_value = pn_value::UNKNOWN;
            child.m_proof = 1;
            child.m_disproof = 1;


            this->m_children[this->m_children_count] = child;
            this->m_children_count += 1;
        }
    }

}

pn_node * pn_node::update_ancestors(pn_node * root){
    pn_node * node = this;

    while(node != root){
        uint32_t old_proof = node->m_proof;
        uint32_t old_disproof = node->m_disproof;

        node->set_proof_and_disproof_numbers();

        if(node->m_proof == old_proof && node->m_disproof == old_disproof){
            return node;
        }

        node = node->m_parent;
    }

    root->set_proof_and_disproof_numbers();
    return root;
}

void pn_node::develop()
{
    assert(this->m_children_count == 0);

    this->generate_all_children();

    for(int i = 0; i < this->m_children_count; i++){
        this->m_children[i].evaluate();
        this->m_children[i].set_proof_and_disproof_numbers();
    }
}

uint32_t sum_proof(pn_node * parent){
    uint32_t sum = 0;

    for(int i = 0; i < parent->m_children_count; i++){
        if(parent->m_children[i].m_proof == UINT32_MAX){
            return UINT32_MAX;
        }

        sum += parent->m_children[i].m_proof;
    }

    return sum;
}

uint32_t sum_disproof(pn_node * parent){
    uint32_t sum = 0;

    for(int i = 0; i < parent->m_children_count; i++){
        if(parent->m_children[i].m_disproof == UINT32_MAX){
            return UINT32_MAX;
        }

        sum += parent->m_children[i].m_disproof;
    }


    return sum;
}

uint32_t min_proof(pn_node * parent){
    uint32_t min = UINT32_MAX;

    for(int i = 0; i < parent->m_children_count; i++){
        if(min > parent->m_children[i].m_proof){
            min = parent->m_children[i].m_proof;
        }
    }

    return min;
}

uint32_t min_disproof(pn_node * parent){
    uint32_t min = UINT32_MAX;

    for(int i = 0; i < parent->m_children_count; i++){
        if(min > parent->m_children[i].m_disproof){
            min = parent->m_children[i].m_disproof;
        }
    }

    return min;
}

//Unsolved node - has finite prood and disproof numbers not equal to zero
//Solving a node either solves or doesnt update its parent


void pn_node::set_proof_and_disproof_numbers(){
    if(this->m_children_count > 0){
        if(this->m_pn_type == pn_type::AND){
            this->m_proof = sum_proof(this);
            this->m_disproof = min_disproof(this);
        } else {
            this->m_proof = min_proof(this);
            this->m_disproof = sum_disproof(this);
        }
    } else {
        if(this->m_pn_value == pn_value::FALSE){
            this->m_proof = UINT32_MAX;
            this->m_disproof = 0;
        } else if(this->m_pn_value == pn_value::TRUE){
            this->m_proof = 0;
            this->m_disproof = UINT32_MAX;
        } else if(this->m_pn_value == pn_value::UNKNOWN){
            this->m_proof = 1;
            this->m_disproof = 1;
        } else {
            assert(false);
        }
    } 

    if(this->m_proof == 0 || this->m_disproof == 0){
        if(this->m_children_count > 0){
            delete [] this->m_children;
            this->m_children_count = 0;
        }


        //for(int i = 0; i < this->m_children.size(); i++){
        //    delete this->m_children[i];
        //}
        //this->m_children.clear();
    }

    //std::cout << "\t Updated proof: " << this->m_proof << " disproof: " << this->m_disproof << std::endl;
}


