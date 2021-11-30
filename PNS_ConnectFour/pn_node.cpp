#include <vector>
#include "bit_board.h"
#include "pn_node.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include "settings.h"

bit_board board;
int pn_nodes_in_exsistence = 0;

pn_node::pn_node(){
    pn_nodes_in_exsistence += 1;
}

pn_node::~pn_node(){
    pn_nodes_in_exsistence -= 1;

    if(this->m_created_children){
        delete [] this->m_children;
    }
}

pn_node * pn_node::select_most_proving() {
    pn_node * node = this;

    while (node->m_expanded) {
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
        board.makemove(node->m_move);
    }

    assert(node->m_children_count == 0);
    //assert(node->m_proof == 1);
    //assert(node->m_disproof == 1);


    return node;
}

void pn_node::evaluate(){
    this->m_pn_value = pn_value::UNKNOWN;

    if(board.haswon(0)){
       //std::cout << "Found a way player 1 wins" << std::endl;
        this->m_pn_value = pn_value::TRUE;
    } else if(board.haswon(1)){
        //std::cout << "Found a way player 1 wins" << std::endl;
        this->m_pn_value = pn_value::FALSE;
    } else if(board.isfull()) {
        this->m_pn_value = pn_value::FALSE;
    //}
    } else if(DEPTH_LIMIT > -1 && board.m_move_num == DEPTH_LIMIT){
        this->m_pn_value = pn_value::FALSE;
    }
}

void pn_node::generate_all_children(){
    pn_type type = pn_type::OR;

    if(this->m_pn_type == type){
        type = pn_type::AND;
    }

    if(this->m_pn_value != pn_value::UNKNOWN){
        return;
    }

    assert(this->m_pn_value == pn_value::UNKNOWN);

    this->m_children = new pn_node[WIDTH+49];
    this->m_created_children = true;

    for(int col = 1; col < WIDTH+1; col++){
        if(board.isplayable(col)){
            pn_node child = pn_node();
            child.m_id = this->m_id + 1;
            child.m_parent = this;
            child.m_move = col;
            child.m_pn_type = type;
            child.m_pn_value = pn_value::UNKNOWN;


            this->m_children[this->m_children_count] = child;
            this->m_children_count += 1;
        }
    }
    
    if(type == pn_type::OR){
        return;
    }

    for(int from = 1; from < WIDTH+1; from++){
        for(int end = 1; end < WIDTH+1; end++){
            if(from == end){
                continue;
            }

            int move = (from * 10) + end;
            if(board.isplayable(move)){
                pn_node child = pn_node();
                child.m_id = this->m_id + 1;
                child.m_parent = this;
                child.m_move = move;
                child.m_pn_type = type;
                child.m_pn_value = pn_value::UNKNOWN;

                this->m_children[this->m_children_count] = child;
                this->m_children_count += 1;
            }
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
        board.undomove();
    }

    root->set_proof_and_disproof_numbers();
    return root;
}

void pn_node::develop()
{
    assert(this->m_children_count == 0);

    this->generate_all_children();

    for(int i = 0; i < this->m_children_count; i++){
        pn_node * child = &this->m_children[i];
        child->evaluate();
        child->set_proof_and_disproof_numbers();

        //if(child->m_pn_type == pn_type::OR && child->m_proof == 0){
        //    break;
        //} else if(child->m_pn_type == pn_type::AND && child->m_disproof == 0){
        //    break;
        //}

        //if(this->m_children[i].pn_type == pn_type::AND){

        //}
    }

    this->m_expanded = true;
}

//Unsolved node - has finite prood and disproof numbers not equal to zero
//Solving a node either solves or doesnt update its parent


void pn_node::set_proof_and_disproof_numbers(){
    if(this->m_expanded){
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
        } 
    } 

   if(this->m_proof == 0 || this->m_disproof == 0){
       if(this->m_created_children){
           this->m_created_children = false;
           delete [] this->m_children;
       }
    }

    //std::cout << "\t Updated proof: " << this->m_proof << " disproof: " << this->m_disproof << std::endl;
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