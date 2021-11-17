#include "bit_board.h"
#include "pn_node.h"
#include <iostream>


int solve(pn_node root) {
  root.evaluate();
  root.set_proof_and_disproof_numbers();

  int i = 0;

  pn_node * current = &root;
  while (root.m_proof != 0 && root.m_disproof != 0) {
    std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof  << "Board ply:" << board.m_move_num << std::endl;

    pn_node * most_proving_node = current->select_most_proving();

   // std::cout << "Most proving move:: " << (int)(most_proving_node->m_move) << std::endl;

    most_proving_node->develop();

    //if(true){
    /*if (i % 1000000 == 0) {
      std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof  << std::endl;

      board.print();
      std::cout << "Has player 0 won ?: " << board.haswon(0) << " - Player 1?: " << board.haswon(1) << std::endl;

      std::cout << "Board0: " << board.m_boards[0] << std::endl;
      std::cout << "Board1: " << board.m_boards[1] << std::endl;


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
    }   */
 
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

  if (root.m_proof == 0) {
    //std::cout << "Root proved true" << std::endl;
    root.m_pn_value = pn_value::TRUE;
    return 1;
  } else if (root.m_disproof == 0) {
    //std::cout << "Root proved false" << std::endl;
    root.m_pn_value = pn_value::FALSE;
    return -1;
  } else {
    //std::cout << "Root proved unknown" << std::endl;
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

  while (infile)
  {
    std::cout << "Another one" << std::endl;

    std::string s;
    if (!std::getline( infile, s )) break;

    board = bit_board();
    board.print();

    int col = 0;

    std::istringstream ss( s );

    int outcome = 0; 

    while (ss)
    {
      std::string s;
      if (!std::getline( ss, s, ',' )) break;

      if(s == "x"){
        board.makemove(col / 6, 0);
      } else if(s == "o"){
        board.makemove(col / 6, 1);
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

    assert((col/6) == 7);

    board.print();
    std::cout << outcome << std::endl;
    std::cout << board.m_boards[0] << std::endl;
    std::cout << board.m_boards[1] << std::endl;



    pn_node root;

    root.m_id = 0;
    root.m_parent = nullptr;
    root.m_pn_type = pn_type::OR;

    int result = solve(root); 

    if(result != 0){
      if(result != outcome){
        std::cout << "Error" << std::endl;
      } else {
        std::cout << "Correct!" << std::endl;
      }
    }
  }
}
*/

int main(){
  std::ifstream infile( "connect-4.data" );

 
  board = bit_board();
  board.print();

  int col = 0;

  std::istringstream ss( "b,b,b,b,b,b,x,b,b,b,b,b,o,b,b,b,b,b,x,o,x,o,x,o,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,draw" );

  int outcome = 0; 

  while (ss)
  {
    std::string s;
    if (!std::getline( ss, s, ',' )) break;

    if(s == "x"){
      board.makemove(col / 6, 0);
    } else if(s == "o"){
      board.makemove(col / 6, 1);
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

  assert((col/6) == 7);

  board.m_move_num = 8;
  board.print();
  std::cout << outcome << std::endl;
  std::cout << board.m_boards[0] << std::endl;
  std::cout << board.m_boards[1] << std::endl;



  pn_node root;
  root.m_id = 0;
  root.m_parent = nullptr;
  root.m_pn_type = pn_type::OR;

  int result = solve(root); 

  std::cout << "Was looking for: " << outcome << std::endl;
  std::cout << "Got: " << result << std::endl;

  
  
}

/*
b,b,b
b,b,b
b,b,b
b,b,b
b,b,o,o,x,o,x,o,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,
b,b,x,x
win
*/

/*
int main() {
  //pointer_test();
  //return 0;

  
  //board.makemove(1);
  //board.makemove(4);
  //board.makemove(1);
  //board.makemove(4);

  board.m_boards[0] = 44056576;
  board.m_boards[1] = 356515840;
  //board.makemove(3);
  //board.makemove(1);
  //board.makemove(0);
  //board.makemove(0);
  //board.makemove(1);
  //board.undomove();
  //board.makemove(1);

  //board.makemove(1, 4);

  //board.print();
  //board.makemove(0, 3);

  //board.makemove(0, 0);
  //board.makemove(0, 0);
  //board.makemove(0, 0);

  // board.makemove(0,6);
  // board.makemove(0, 0);

  // board.makemove(0, 1);
  // board.makemove(0, 1);
  // board.makemove(0, 1);

 

  //std::cout << board.haswon(1) << std::endl; 
  pn_node root;

  root.m_id = 0;
  root.m_parent = nullptr;
  //root.m_board = board;
  root.m_pn_type = pn_type::OR;

  solve(root); 

}
*/