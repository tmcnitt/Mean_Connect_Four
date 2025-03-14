#include "bit_board.h"
#include "pn_node.h"
#include <iostream>
#include "settings.h"
#include <bitset>
#include <cassert>

int solve(pn_node root) {
  //root.evaluate();
  root.set_proof_and_disproof_numbers();

  int i = 0;

  pn_node * current = &root;
  while (root.m_proof != 0 && root.m_disproof != 0) {
    //std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof  << "Board ply:" << board.m_move_num << std::endl;

    pn_node * most_proving_node = current->select_most_proving();

    //assert(!board.haswon(0) && !board.haswon(1));
    //std::cout << "Most proving move:: " << (int)(most_proving_node->m_move) << std::endl;

    most_proving_node->develop();

    if(i % 10000 == 0){
      if(!board.check_gravity()){
        std::cout << "Gravity validation in solve" << std::endl;
        board.print();
        assert(false);
        return 0;
      };
    }

    //if(true){
    if (i % 1000000 == 0) {
    //if(false){
      std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof  << std::endl;
      std::cout << "Nodes in exsistence: " << pn_nodes_in_exsistence << " Games won: " << games_won << " Games lost: " << games_lost << std::endl;
      std::cout << "Root children proofs: ";
      for(int i = 0; i < root.m_children_count; i++){
        std::cout << "(" << root.m_children[i].m_proof << "," << root.m_children[i].m_disproof << "), ";
      }
      std::cout << std::endl;

      board.print();
      std::cout << "Has player 0 won ?: " << board.haswon(0) << " - Player 1?: " << board.haswon(1) << std::endl;

      pn_node * check_history = most_proving_node->m_parent;
      while(check_history != nullptr){
        std::cout << "ID: " << (int)check_history->m_id << " Move for this node: " << (int)check_history->m_move << std::endl;
        check_history = check_history->m_parent;
      }
      

    
      pn_node *check = most_proving_node;
      while (check != nullptr) {
        std::cout << "Depth: " << (int)check->m_id
                  << " Proof: " << check->m_proof
                  << " Disproof: " << check->m_disproof;

        if (check->m_pn_type == pn_type::OR) {
          std::cout << " - OR Node";
        } else {
          std::cout << " - AND Node";
        }

        if (check->m_pn_value == pn_value::TRUE) {
          std::cout << " - Proven TRUE";
        } else if (check->m_pn_value == pn_value::FALSE) {
          std::cout << " - Proven FALSE";
        } else {
          std::cout << " - Proven UNKNOWN";
        }

        std::cout << " - Num children: " << (int)check->m_children_count;
        std::cout << std::endl;

        check = check->m_parent;
      } 
    }  
 
    current = most_proving_node->update_ancestors(&root);


    i += 1;

   //if(most_proving_node->m_id > 2){
   //  break;
   //}

    // if(i % 1 == 0){
    /*  std::cout << "Selected node: " << most_proving_node->m_id << std::endl;
      std::cout <<
        "Selected proof: " << most_proving_node->m_proof <<
        " Disproof: " << most_proving_node->m_disproof <<
        " Expanded?: " << most_proving_node->m_expanded;

      if(most_proving_node->m_pn_value == pn_value::FALSE){
        std::cout << " Value: False" << std::endl;
      }

      if(most_proving_node->m_pn_value == pn_value::TRUE){
        std::cout << " Value: True" << std::endl;
      }

      if(most_proving_node->m_pn_value == pn_value::UNKNOWN){
        std::cout << " Value: Unknown" << std::endl;
      } */

    //}

    // if(i > 50){
    //  break;
    //}
  }

  std::cout << board.m_move_num << std::endl;
  std::cout << "final root proof: " << root.m_proof << " disproof: " << root.m_disproof << std::endl;

  if (root.m_proof == 0) {
    std::cout << "Root proved true" << std::endl;
    root.m_pn_value = pn_value::TRUE;
    return 1;
  } else if (root.m_disproof == 0) {
    std::cout << "Root proved false" << std::endl;
    root.m_pn_value = pn_value::FALSE;
    return -1;
  } else {
    std::cout << "Root proved unknown" << std::endl;
    root.m_pn_value = pn_value::UNKNOWN;
    return 0;
  }

  std::cout << "Final nodes: " << pn_nodes_in_exsistence << std::endl;
}

#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
/*
int main(){
  std::ifstream infile( "connect-4.data" );

  int i = 0;
  while (infile)
  {
    std::cout << "Another one" << std::endl;

    std::string s;
    if (!std::getline( infile, s )) break;

    board = bit_board();

    int col = 0;

    std::istringstream ss( s );

    int outcome = 0; 

    while (ss)
    {
      std::string s;
      if (!std::getline( ss, s, ',' )) break;

      if(s == "x"){
        board.makemove_setup_only_dont_use(col / 6, 0);
      } else if(s == "o"){
        board.makemove_setup_only_dont_use(col / 6, 1);
      }

      if(s == "win"){
        outcome = 1;
      }
      if(s == "loss"){
        outcome = -1;
      }
      if(s == "draw"){
        outcome = 0;
      }

      col += 1;
    }

    i += 1;
    if(i == 1){
      continue;
    }

    board.m_pieces_on_board = 8;
    board.m_move_num = 8;


    assert((col/6) == 7);

    board.print();

    pn_node root;

    root.m_id = 0;
    root.m_parent = nullptr;
    root.m_pn_type = pn_type::OR;

    int result = solve(root); 

    std::cout << "Wanted to get: " << outcome << std::endl;
    std::cout << "Got: " << result << std::endl;

    break;
  }
}  */

/*
b,b,b
b,b,b
b,b,b
b,b,b
b,b,o,o,x,o,x,o,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,
b,b,x,x
win
*/




int main() {
  pn_node root;

  
  root.m_id = 0;
  root.m_parent = nullptr;
  //root.m_board = board;
  root.m_pn_value = pn_value::UNKNOWN;
  root.m_move = 255;
  root.m_pn_type = pn_type::OR;

  solve(root); 

}