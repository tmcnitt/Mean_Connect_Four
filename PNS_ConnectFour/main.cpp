#include "bit_board.h"
#include "pn_node.h"
#include <iostream>
#include "settings.h"
#include <bitset>

int solve(pn_node root) {
  root.evaluate();
  root.set_proof_and_disproof_numbers();

  int i = 0;

  pn_node * current = &root;
  while (root.m_proof != 0 && root.m_disproof != 0) {
    //std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof  << "Board ply:" << board.m_move_num << std::endl;

    pn_node * most_proving_node = current->select_most_proving();

   // std::cout << "Most proving move:: " << (int)(most_proving_node->m_move) << std::endl;

    most_proving_node->develop();


    //if(true){
    if (i % 1000000 == 0) {
      std::cout << "Iteration: " << i << " Root proof: " << root.m_proof << " Root disproof: " << root.m_disproof  << std::endl;

      board.check_gravity();
      board.print();
      std::cout << "Has player 0 won ?: " << board.haswon(0) << " - Player 1?: " << board.haswon(1) << std::endl;

      pn_node * check_history = most_proving_node->m_parent;
      while(check_history != nullptr){
        std::cout << "ID: " << (int)check_history->m_id << " Move for this node: " << (int)check_history->m_move << std::endl;
        check_history = check_history->m_parent;
      }
      

      if(!board.check_gravity()){
        std::cout << "Gravity validation in solve" << std::endl;
        assert(false);
      };

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

    std::cout << "Wanted to get: " << outcome << std::endl;
    std::cout << "Got: " << result << std::endl;
  }
} */

/*
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
*/

/*
b,b,b
b,b,b
b,b,b
b,b,b
b,b,o,o,x,o,x,o,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,
b,b,x,x
win
*/

int test();
int test_bug();
int test_gravity();
int test_gravity_2();
int test_bug_floating();
int test_gravity_low();


int main() {
  return test();
  //return test_gravity_2();
  //return test_gravity_2();
  //return test_bug_floating();

  //return test_gravity_2();
  //return test_gravity();
  //return test();
  //return test_bug();
  //std::cout << DEPTH_LIMIT << std::endl;
  //pointer_test();
  //return 0;

  
  //board.makemove(1);
  //board.makemove(4);
  //board.makemove(1);
  //board.makemove(4);

  //board.m_boards[0] = 44056576;
  //board.m_boards[1] = 356515840;
  //board.makemove(3);
  //board.makemove(1);
  //board.makemove(4);
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
  root.m_move = 255;
  root.m_pn_type = pn_type::OR;

  solve(root); 

} 

int test_gravity_low(){
  board.makemove(1);
  board.m_heights[0] = 0;
  std::cout << (int)board.m_heights[0] << std::endl;
  board.check_gravity();
}

int test_bug_floating(){
  /*board.makemove(5);
  board.print();

  std::bitset<64> y(board.m_boards[0]);
  std::cout << y << std::endl;

  //board.undomove(); */

  //assert(board.m_boards[0] == 2216471560192);
  board.m_boards[0] = 2216471560192;
  
  assert(!board.check_gravity());
  //std::bitset<64> x(board.m_boards[0]);
  //std::cout << x << std::endl;


  //board.makemove(5);
  //board.makemove(51);
  //board.m_heights[0] = 1;
  //board.print();
  //board.makemove(51);
}

int test_gravity_2(){
  board.m_boards[0] = (uint64_t)34359738368;
  board.m_boards[1] = 0;

  board.m_heights[0] = (char)0;
  board.m_heights[1] = (char)7;
  board.m_heights[2] = (char)14;
  board.m_heights[3] = (char)21;
  board.m_heights[4] = (char)28;
  board.m_heights[5] = (char)36;
  board.m_heights[6] = (char)42;
  board.m_pieces_on_board = 1;
  board.m_move_num = 1;
  board.print();
  assert(board.check_gravity());
  assert(board.isplayable(61));


  board.makemove(61);
  board.print();

  assert(board.check_gravity());

  return 0 ;


//Board0: 5497602179072
//Board1: 1108122533888
//Heights: 0,7,14,26,28,35,43,
}

int test_gravity(){
  board.m_boards[0] = 5497602179072;
  board.m_boards[1] = 1108122533888;
  board.m_heights[0] = 0;
  board.m_heights[1] = 7;
  board.m_heights[2] = 14;
  board.m_heights[3] = 26;
  board.m_heights[4] = 28;
  board.m_heights[5] = 35;
  board.m_heights[6] = 43;

  board.print();

  board.check_gravity();

  return 0 ;


//Board0: 5497602179072
//Board1: 1108122533888
//Heights: 0,7,14,26,28,35,43,
}

int test_bug(){
  board.makemove_setup_only_dont_use(0, 0);
  board.makemove_setup_only_dont_use(0, 0);
  board.makemove_setup_only_dont_use(0, 1);
  board.makemove_setup_only_dont_use(0, 0);
  board.print();
  assert(!board.isplayable(61));
}

int test(){
  board.makemove(1);
  board.makemove(1);
  board.makemove(1);
  board.makemove(2);
  board.makemove(2);
  board.makemove(2);
  board.makemove(3);
  board.makemove(4);
  board.makemove(5);
  board.makemove(6);

  std::cout << "Before mean move" << std::endl;
  board.print();

  assert(!board.isplayable(76));
  assert(!board.isplayable(54));
  assert(board.isplayable(21));

  board.makemove(21);

  std::cout << "After mean move" << std::endl;
  board.print();

  board.undomove();

  std::cout << "After undo" << std::endl;
  board.print();

  board.makemove(1);
  board.makemove(2);

  std::cout << "Place ontop off" << std::endl;
  board.print();

  board.undomove();
  board.undomove();

  std::cout << "After undo" << std::endl;
  board.print();

  board.makemove(1);
  board.makemove(1);
  board.makemove(1);

  std::cout << "Filled col one" << std::endl;
  board.print();

  assert(!board.isplayable(1));
  assert(!board.isplayable(21));
  assert(!board.isplayable(31));

}