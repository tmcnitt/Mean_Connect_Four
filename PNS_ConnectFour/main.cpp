#include "bit_board.h"
#include "pn_node.h"
#include <iostream>


void solve(pn_node root) {
  root.evaluate();
  root.set_proof_and_disproof_numbers();

  int i = 0;

  pn_node * current = &root;
  while (root.m_proof != 0 && root.m_disproof != 0) {

    pn_node * most_proving_node = current->select_most_proving();

    most_proving_node->develop();


    if(true){
    //if (i % 1000000 == 0) {
      std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof << " Nodes out there: " << pn_nodes_in_exsistence << std::endl;
      most_proving_node->m_board.print();
      std::cout << "Has player 0 won ?: " << most_proving_node->m_board.haswon(0) << " - Player 1?: " << most_proving_node->m_board.haswon(1) << std::endl;

      std::cout << "Board0: " << most_proving_node->m_board.m_boards[0] << std::endl;
      std::cout << "Board1: " << most_proving_node->m_board.m_boards[1] << std::endl;


      pn_node *check = most_proving_node;
      while (check != nullptr) {
        std::cout << "Depth: " << check->m_id
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

        std::cout << " - Num children: " << check->m_children_count;
        std::cout << std::endl;

        check = check->m_parent;
      }
    }

    current = most_proving_node->update_ancestors(&root);


    i += 1;

    if(most_proving_node->m_id > 1){
      break;
    }

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

  if (root.m_proof == 0) {
    std::cout << "Root proved true" << std::endl;
    root.m_pn_value = pn_value::TRUE;
  } else if (root.m_disproof == 0) {
    std::cout << "Root proved false" << std::endl;
    root.m_pn_value = pn_value::FALSE;
  } else {
    std::cout << "Root proved unknown" << std::endl;
    root.m_pn_value = pn_value::UNKNOWN;
  }

  std::cout << "Final nodes: " << pn_nodes_in_exsistence << std::endl;
}


int main() {
  //pointer_test();
  //return 0;

  bit_board board;
  //board.makemove(0, 3);
  //board.makemove(0, 3);

  //board.makemove(0, 0);
  //board.makemove(0, 0);
  //board.makemove(0, 0);

  // board.makemove(0,6);
  // board.makemove(0, 0);

  // board.makemove(0, 1);
  // board.makemove(0, 1);
  // board.makemove(0, 1);

 

  //std::cout << board.haswon(1) << std::endl; */

  pn_node root;

  root.m_id = 0;
  root.m_parent = nullptr;
  root.m_board = board;
  root.m_pn_type = pn_type::OR;

  solve(root);
}
