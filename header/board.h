#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <random>
#include <chrono>
#include <locale> 
#include <cstdlib>

#include "graph.h"
#include "ComputerAI.h"

enum PLAYERS{NOBODY = 0, PLAYER, COMPUTER};

const int CLEAR_SCREEN = 50;
const int MIN_BOARD_SIZE = 3;
const int MAX_BOARD_SIZE = 12;

//static std::mt19937 Mersenne(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

class Board
{
    private:
        //General info
        int  m_board_size;
        int  m_num_nodes;
        bool m_shut_down;

        //Board representation
		std::vector<PLAYERS> m_hex_board;               //Vector with the board and the pieces
        Graph m_graph_board;                            //Graph representation of the board

        //Player data
        std::map<int, char> m_player_hexes;             //Dictionary associating num of player and his piece type: O or X
        PLAYERS m_player;                               //Current player

        //Winner data
        int m_hex_to_check;                             //Is the node that rules the search for a winner
		std::vector<int>  m_tree;                       //Tree used to check if there is a winner.
        std::vector<Edge> adjacent_edges;               //The adjacent edges of a node.
        std::vector<int>  posible_edges;                //A queue of posible edges used in the search tree algortihm.
        std::vector<int>  nodes_to_visit;               //A queue of nodes that are not visited yet

		//Simulation data
		std::vector<int>  empty_hex;                    //The empty squares used to make the simulation using random shuffle

        //AI data
        ComputerAI AI;                                  //AI object
        

    public:

        Board();
        Board(const Board &b);
        virtual ~Board();

        //Main loop functions
        void Run();
        void Init(int board_size);
        void DrawIntro();
        void Draw();
        void Input();
        inline void ChangePlayer() {if(m_player == PLAYER) m_player = COMPUTER; else m_player = PLAYER;}

        //Winner functions
        void SearchNodeTree(const PLAYERS player);
        PLAYERS IsWinner();
        PLAYERS IsWinnerComputer();

        //Simularion movements
		void MakeRandomMovement();
        void MakeComputerMovement(int movement);

        //Getters and setters
        inline int BoardSize() {return m_board_size;}
        inline bool Running() {return !m_shut_down;}
		inline const std::vector<PLAYERS>& HexBoard() const { return m_hex_board; }
        inline char CurrentPlayer() {return m_player;}
		inline std::vector<int>& EmptyHex() { return empty_hex; }
		inline void SetEmptyHex(std::vector<int> &ee) { empty_hex = ee; }
        inline void SetHexToCheck(int hex_to_check) {m_hex_to_check = hex_to_check;}
};

#endif // BOARD_H
