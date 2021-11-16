#include <vector>
#include "bit_board.h"
#include "pn_node.h"
#include <algorithm>
#include <iostream>
#include <cassert>

pn_node * pn_node::select_most_proving() {
    pn_node * node = this;

    while (!node->m_children.empty()) {
        int i = 0;

        if (node->m_pn_type == pn_type::OR) {
            while (node->m_children[i]->m_proof != node->m_proof) {
                i += 1;
            }
        } else {
            while (node->m_children[i]->m_disproof != node->m_disproof) {
                i += 1;
            }
        }

        node = node->m_children[i];
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
    pn_type type = pn_type::AND;

    if(this->m_pn_type == type){
        type = pn_type::OR;
        player = 0;
    }

    if(this->m_pn_value != pn_value::UNKNOWN){
        return;
    }
    assert(this->m_pn_value == pn_value::UNKNOWN);

    for(int col = 0; col < WIDTH; col++){
        if(this->m_board.isplayable(col)){
            bit_board child_board(this->m_board);
            child_board.makemove(player, col);


            pn_node * child = new pn_node();
            child->m_id = this->m_id + 1;
            child->m_parent = this;
            child->m_board = child_board;
            child->m_pn_type = type;

            //std::cout << "Adding node with ids: " << child->m_id << std::endl;

            this->m_children.push_back(child);
        }
    }

}

void pn_node::update_ancestors(){
    pn_node * node = this;

    while(node != nullptr){
        node->set_proof_and_disproof_numbers();
        node = node->m_parent;
    }
}

void pn_node::develop()
{
    this->generate_all_children();
    for(pn_node * child: this->m_children){
        child->evaluate();
        child->set_proof_and_disproof_numbers();
    }
}

uint32_t sum_proof(pn_node * parent){
    uint32_t sum = 0;

    for(pn_node * child : parent->m_children){
        if(child->m_proof == UINT32_MAX){
            return UINT32_MAX;
        }

        sum += child->m_proof;
    }

    return sum;
}

uint32_t sum_disproof(pn_node * parent){
    uint32_t sum = 0;

    for(pn_node * child : parent->m_children){
        if(child->m_disproof == UINT32_MAX){
            return UINT32_MAX;
        }
        
        sum += child->m_disproof;
    }

    return sum;
}

uint32_t min_proof(pn_node * parent){
    uint32_t min = UINT32_MAX;

    for(pn_node * child : parent->m_children){
        if(min > child->m_proof){
            min = child->m_proof;
        }
    }

    return min;
}

uint32_t min_disproof(pn_node * parent){
    uint32_t min = UINT32_MAX;

    for(pn_node * child : parent->m_children){
        if(min > child->m_disproof){
            min = child->m_disproof;
        }
    }

    return min;
}

//Unsolved node - has finite prood and disproof numbers not equal to zero
//Solving a node either solves or doesnt update its parent


void pn_node::set_proof_and_disproof_numbers(){
    //std::cout << "Updating proof and disproof numbers for " << this->m_id << std::endl;
    //std::cout << "\t is expanded: " << this->m_expanded << std::endl;
    //std::cout << "\t num children: " << this->m_children.size() << std::endl;

    //if (this->m_pn_type == pn_type::AND){
    //    std::cout << "\t pn_type:  AND"  << std::endl;
    //} else {
    //    std::cout << "\t pn_type:  OR"  << std::endl;
    //}
    if(!this->m_children.empty()){
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

    //std::cout << "\t Updated proof: " << this->m_proof << " disproof: " << this->m_disproof << std::endl;
}


