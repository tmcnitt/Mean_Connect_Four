#include "bit_board.h"
#include "pn_node.h"
#include <iostream>

void solve(pn_node root){
  root.evaluate();
  root.set_proof_and_disproof_numbers();

  int i = 0;

  while(root.m_proof != 0 && root.m_disproof != 0){
    pn_node * most_proving_node = root.select_most_proving();

    //std::cout << "Depth: " << most_proving_node->m_id << " Selected proof: " << most_proving_node->m_proof << " Disproof: " << most_proving_node->m_disproof << std::endl;

    most_proving_node->develop();

    most_proving_node->update_ancestors();

    i += 1;

    //if(i % 1 == 0){
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

      //std::cout << "Iteration: " << i << " Proof: " << root.m_proof << " Disproof: " << root.m_disproof << std::endl;
    //}

    //if(i > 1){
    //  break;
    //}
  }

  if(root.m_proof == 0){
    std::cout << "Root proved true" << std::endl;
    root.m_pn_value = pn_value::TRUE;
  } else if(root.m_disproof == 0){
    std::cout << "Root proved false" << std::endl;
    root.m_pn_value = pn_value::FALSE;
  } else {
    std::cout << "Root proved unknown" << std::endl;
    root.m_pn_value = pn_value::UNKNOWN;
  }
}

int main()
{
  bit_board board;

  //board.makemove(0, 0);
  //board.makemove(0, 0);
  //board.makemove(0, 0);

  //board.makemove(0, 1);
  //board.makemove(0, 1);
  //board.makemove(0, 1);


  pn_node root;
  
  root.m_id = 0;
  root.m_parent = nullptr;
  root.m_board = board;
  root.m_pn_type = pn_type::AND;
  

  solve(root);
}
