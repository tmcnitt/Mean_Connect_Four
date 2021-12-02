#include <vector>
#include "bit_board.h"
#include "pn_node.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include "settings.h"

bit_board board;
uint64_t pn_nodes_in_exsistence = 0;
uint64_t games_won = 0;
uint64_t games_lost = 0;

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


    if(!node->m_expanded){
        return this;
    }

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


    assert(this->m_id != node->m_id);
    assert(node->m_children_count == 0);
    assert(board.m_moves[board.m_move_num-1] == node->m_move);
    //assert(node->m_proof == 1);
    //assert(node->m_disproof == 1);


    return node;
}

void pn_node::evaluate(){
    assert(this->m_id == board.m_move_num);
    assert(board.m_moves[board.m_move_num-1] == this->m_move);

    this->m_pn_value = pn_value::UNKNOWN;

    if(board.haswon(0)){
        //Win, we get to go to the next round
        this->m_pn_value = pn_value::TRUE;
        games_won++;
    } else if(board.haswon(1)){
        //Lose, we dont go on :(
        this->m_pn_value = pn_value::FALSE;
        games_lost++;

    //Draws, we go on to the next round
    } else if(board.isfull()) {
        this->m_pn_value = pn_value::TRUE;
    } else if(this->m_id >= 100){
        this->m_pn_value = pn_value::TRUE;
    } else if(DEPTH_LIMIT > -1 && this->m_id > DEPTH_LIMIT){
        this->m_pn_value = pn_value::TRUE;
    } 
}

void pn_node::generate_all_children(){
    pn_type type = pn_type::OR;

    //If this node is an OR, its children are AND
    if(this->m_pn_type == pn_type::OR){
        type = pn_type::AND;
    }

    //AND -> Need to prove all children true (children = oponent moves)
    //OR -> Only need to prove one child true (children = our moves)

    if(this->m_pn_value != pn_value::UNKNOWN){
        return;
    }

    assert(this->m_pn_value == pn_value::UNKNOWN);

    int valid_moves = 0;
    for(int col = 1; col < WIDTH+1; col++) {
        if (board.isplayable(col)) {
            valid_moves++;
        }
    }

    bool can_use_mean_moves = false;

    if(ENABLE_MEAN){
        //Mean move enabled. 
        if(type == pn_type::OR) {
            //Oponnent can always use them
            //assert(board.m_move_num % 2 == 0);
            can_use_mean_moves = true;
        } else if(PLAYER_ONE_MEAN) {
            //We should be good to go
            can_use_mean_moves = true;

            //Except if these limits are on

            if(PLAYER_ONE_MEAN_COUNT_LIMIT != -1){
                can_use_mean_moves &= board.popcount() <= PLAYER_ONE_MEAN_COUNT_LIMIT;
            } 

             if(PLAYER_ONE_MEAN_DEPTH_LOWER_LIMIT != -1){
                can_use_mean_moves &= this->m_id > PLAYER_ONE_MEAN_DEPTH_LOWER_LIMIT;
            }

            if(PLAYER_ONE_MEAN_DEPTH_UPPER_LIMIT != -1){
                can_use_mean_moves &= this->m_id < PLAYER_ONE_MEAN_DEPTH_UPPER_LIMIT;
            }
        }
    }
    
    

    if(can_use_mean_moves){
        for(int from = 1; from < WIDTH+1; from++) {
            for (int end = 1; end < WIDTH + 1; end++) {
                if (from == end) {
                    continue;
                }

                int move = (from * 10) + end;
                if (board.isplayable(move)) {
                    valid_moves++;
                }
            }
        }
    }

    this->m_children = new pn_node[valid_moves];
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
    
    if(can_use_mean_moves) {
        for (int from = 1; from < WIDTH + 1; from++) {
            for (int end = 1; end < WIDTH + 1; end++) {
                if (from == end) {
                    continue;
                }

                int move = (from * 10) + end;
                if (board.isplayable(move)) {
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
}

pn_node * pn_node::update_ancestors(pn_node * root){
    pn_node * node = this;

    while(node != root){
        uint64_t old_proof = node->m_proof;
        uint64_t old_disproof = node->m_disproof;

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

        board.makemove(child->m_move);
        child->evaluate();
        board.undomove();

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
            this->m_proof = UINT64_MAX;
            this->m_disproof = 0;
        } else if(this->m_pn_value == pn_value::TRUE){
            this->m_proof = 0;
            this->m_disproof = UINT64_MAX;
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

uint64_t sum_proof(pn_node * parent){
    uint64_t sum = 0;

    for(int i = 0; i < parent->m_children_count; i++){
        if(parent->m_children[i].m_proof == UINT64_MAX){
            return UINT64_MAX;
        }

        sum += parent->m_children[i].m_proof;
    }

    return sum;
}

uint64_t sum_disproof(pn_node * parent){
    uint64_t sum = 0;

    for(int i = 0; i < parent->m_children_count; i++){
        if(parent->m_children[i].m_disproof == UINT64_MAX){
            return UINT64_MAX;
        }

        sum += parent->m_children[i].m_disproof;
    }


    return sum;
}

uint64_t min_proof(pn_node * parent){
    uint64_t min = UINT64_MAX;

    for(int i = 0; i < parent->m_children_count; i++){
        if(min > parent->m_children[i].m_proof){
            min = parent->m_children[i].m_proof;
        }
    }

    return min;
}

uint64_t min_disproof(pn_node * parent){
    uint64_t min = UINT64_MAX;

    for(int i = 0; i < parent->m_children_count; i++){
        if(min > parent->m_children[i].m_disproof){
            min = parent->m_children[i].m_disproof;
        }
    }

    return min;
}