#include "board.h"

#include <string>
#include <boost/lexical_cast.hpp>



Board::Board() : m_shut_down(false), m_hex_to_check(0)
{
    m_player_hexes[PLAYER] = 'X' ;
    m_player_hexes[COMPUTER] = 'O';
    m_player_hexes[NOBODY] = '.';
    m_player = PLAYER;
}

//WE NEED TO MAKE COPIES OF THE BOARD SO WE USE THE COPY CONSTRUCTOR
Board::Board(const Board &b) : m_board_size(b.m_board_size), m_num_nodes(b.m_num_nodes), m_shut_down(b.m_shut_down),
                               m_hex_board(b.m_hex_board), m_graph_board(b.m_graph_board), m_player_hexes(b.m_player_hexes),
                               m_player(b.m_player) , m_hex_to_check(b.m_hex_to_check),adjacent_edges(b.adjacent_edges),
                               posible_edges(b.posible_edges), nodes_to_visit(b.nodes_to_visit), m_tree(b.m_tree), AI(b.AI),
                               empty_hex(b.empty_hex)
{


}


Board::~Board()
{

}

//MAIN LOOP
void Board::Run()
{
    DrawIntro();
    Draw();

    while(Running())
    {
        Input();
        Draw();

        if(IsWinner() == false) ChangePlayer();
		else
		{
			std::cout << std::endl << "PLAYER " << m_player_hexes[m_player] << " WINS!!! " << std::endl << std::endl;
			int i = 0;  std::cin >> i;
		}
    }
}

//INIT THE GRAPH DEPENDING ON THE GIVEN SIZE
void Board::Init(int board_size)
{
    m_board_size = board_size;
	std::vector<PLAYERS> hex_board_init(m_board_size*m_board_size, NOBODY);
    m_hex_board.swap(hex_board_init);

    Graph graph_init(m_board_size);
	m_graph_board = graph_init;

	m_num_nodes = m_graph_board.GetNumNodes();
	
    for(int i = 0; i < m_num_nodes; i++) empty_hex.push_back(i);
}


//CHOOSE THE SIZE OF THE BOARD
void Board::DrawIntro()
{
    std::cout << "GAME OF HEX" << std::endl << std::endl ;

    bool range_ok = false;
    std::string  board_size = "e2";
    int int_size = 0;
	std::locale loc;

    while(range_ok == false)
    {
        std::cout << "Enter a board size between 3 and 12: " ;
        std::cin >> board_size;

        std::string substraction = "e2";
        substraction = board_size.substr(0,2);

        //Check if it's a valid digit
		if ((std::isdigit(substraction[0], loc) == true) && (std::isdigit(substraction[1], loc) == true) && (board_size.size() == 2))
        {
            int_size = boost::lexical_cast<int>(substraction);
        }
		else if (std::isdigit(substraction[0], loc) == true && board_size.size() == 1)
        {
            int_size = boost::lexical_cast<int>(substraction[0]);
        }

        //Check if the digit it's between the accepted values
        if(int_size >= MIN_BOARD_SIZE && int_size <= MAX_BOARD_SIZE) range_ok = true;
        else std::cout << "Wrong input." << std::endl;
    }

    Init(int_size);
}

//DRAW BOARD DEPENDING ON THE SIZE
void Board::Draw()
{
    //Clear screen
    for (int i = 0; i < CLEAR_SCREEN; i++ ) std::cout << '\n';

    std::cout << "GAME OF HEX" << std::endl << std::endl ;
    std::cout << "a) Player X must connect the left part of the board with the right part" << std::endl;
    std::cout << "b) Player O must connect the up part of the board with the down part" << std::endl;
    std::cout << "c) Follow the edges to know the connections on the board" << std::endl;
    std::cout << "d) To put the piece on the board follow the row number and the column letter." << std::endl;
    std::cout << "   First introduce the row number and after the column letter, then press Enter" << std::endl;
    std::cout << "   Example: 2b  |  2 corresponds to the row and b to the column " << std::endl << std::endl;

    //Write up column letter
    for (int i = 0; i < m_board_size; i++ ) std::cout << "  " << static_cast<char>(i+97) << " ";
    std::cout << std::endl << std::endl;

    for (int i = 0; i < m_num_nodes; i++ )
    {

		int row = i / m_board_size;
		int column = i % m_board_size;

		//_____________________________________________________
		//Tabs for horizontal
		if (column == 0)
		{
			for (int m = row; m != 0; m--)
			{
				std::cout << "  ";
			}

			//Row number on the left
			if (row < 9) std::cout << row + 1 << "   ";
			else std::cout << row + 1 << "  ";
		}
			

        //Write pieces of the board and horizontal edges
		std::cout << m_player_hexes[m_hex_board[i]];
        if((column+1) % m_board_size != 0 ) std::cout << " - ";
        
		if ((column + 1) % m_board_size == 0)
		{
			//Row number on the right
			std::cout << "   " << row + 1 << std::endl;

			for (int m = row; m != 0; m--) std::cout << "  ";
			std::cout << "    ";

			//Write diagonal and vertical edges
			
			if ((row + 1) % m_board_size != 0)
			{
				for (int n = 0; n < m_board_size; n++)
				{
					std::cout << " \\";
					if (n < m_board_size - 1)std::cout << " /";
				}
			}
		}

		if ((i + 1) % m_board_size == 0) std::cout << std::endl;
    }

    //Write down column letter
    for (int i = 0; i <= m_board_size + 1; i++ )
    {
        std::cout  << "  ";
    }

    for (int i = 0; i < m_board_size; i++ )
    {
        std::cout  << static_cast<char>(i+97) << "   ";
    }
    std::cout << std::endl << std::endl;
}

//TAKES THE MOVEMENT INPUT AND CHECKS IF IT'S CORRECT
void Board::Input()
{
    std::string input = "empty";
    int row = 0;
    int column = 0;
    bool is_posible_movement = false;
    bool is_correct_input = false;
	std::locale loc;


    if(m_player == PLAYER)
    {
        while(is_posible_movement == false)
        {
            std::cout << "Insert movement for player " << m_player_hexes[m_player] << ": ";
            std::cin >> input;

            //stoi doesn't work in the GNU GCC compiler, must be solved by the developers.
            //To make the conversion string to int I use lexical_cast from boost library

            if(input.size() == 2)
            {
				if (std::isdigit(input[0], loc) == true)
                {
                    row = boost::lexical_cast<int>(input[0])-1;
                    column = boost::lexical_cast<int>(input[1]-96)-1;
                    is_correct_input = true;
                }
                else is_correct_input = false;
            }
            else if(m_board_size >= 10 && input.size() == 3 )
            {
                std::string substraction = input.substr(0,2);
				if (std::isdigit(substraction[0], loc) && std::isdigit(substraction[1], loc))
                {
                    column = boost::lexical_cast<int>(input[2]- 96)-1;
                    row = boost::lexical_cast<int>(substraction)-1;
                    is_correct_input = true;
                }
                else is_correct_input = false;
            }

            if(row < m_board_size && column < m_board_size && row >= 0 && column >= 0)
            {
				int movement = row*m_board_size + column;
				if (m_hex_board[movement] == NOBODY && is_correct_input == true)
                {
					m_hex_board[movement] = PLAYER;
					m_hex_to_check = movement;
                    is_posible_movement = true;
                    std::vector<int>::iterator itr = find(empty_hex.begin(), empty_hex.end(), movement );
                    empty_hex.erase(itr);
                }
                else { std::cout << "Movement not possible, try another one" << std::endl << std::endl;}
            }
            else { std::cout << "Movement not possible, try another one" << std::endl << std::endl;}
        }  
    }
    else if(m_player == COMPUTER)
    {
        int node = AI.BestMovement(this); // We pass a pointer to the board to make the montecarlo simulation
		m_hex_board[node] = COMPUTER;
        m_hex_to_check = node;
		std::vector<int>::iterator itr = find(empty_hex.begin(), empty_hex.end(), node);
        empty_hex.erase(itr);
    }
    std::cout << std::endl;
}



void Board::MakeRandomMovement()
{
	int pos = 0;
	m_player = COMPUTER;

	std::random_shuffle(empty_hex.begin(), empty_hex.end()); //Too slow

    //Fill the board at random
	while (!empty_hex.empty())
    {
        ChangePlayer();
		pos = *(empty_hex.end() - 1);
		empty_hex.pop_back();
        m_hex_board[pos] = m_player; //PLAYER OR COMPUTER
    }

}


void Board::MakeComputerMovement(int movement)
{
    m_hex_board[movement] = COMPUTER;
    std::vector<int>::iterator itr = find(empty_hex.begin(), empty_hex.end(), movement );
    empty_hex.erase(itr);
}


//SEARCH THE TREE TO FIND IF THERE IS A WINNER AND RETURNS THE WINNER
PLAYERS Board::IsWinner()
{
	bool touch_up = false;
	bool touch_down = false;
	int movement = 0;

	m_tree.clear();

	m_hex_to_check = m_num_nodes + VIRTUAL_NODE::NORTH_NODE; //NORTH VIRTUAL NODE
	SearchNodeTree(COMPUTER);

	for (auto tree_b = m_tree.begin(), tree_e = m_tree.end(); tree_b != tree_e; ++tree_b)
	{
		movement = *tree_b;

		if (movement  < m_num_nodes) //If it's not a virtual node
		if (m_hex_board[movement] == COMPUTER)
		{
			if (movement/m_board_size == 0) touch_up = true;
			if (movement/m_board_size == m_board_size - 1) touch_down = true;
		}

		if (touch_up == true && touch_down == true)
		{
			m_shut_down = true;
			return COMPUTER;
		}
	}

	bool touch_right = false;
	bool touch_left = false;

	m_tree.clear();

	m_hex_to_check = m_num_nodes + VIRTUAL_NODE::WEST_NODE; //WEST VIRTUAL NODE
	SearchNodeTree(PLAYER);

	//Check if the tree touches the edges of the board
	for (auto tree_b = m_tree.begin(), tree_e = m_tree.end(); tree_b != tree_e; ++tree_b)
	{
		movement = *tree_b;

		if (movement < m_num_nodes) //If it's not a virtual node
		if (m_hex_board[movement] == PLAYER)
		{
			if ((movement + 1) % m_board_size == 0) touch_right = true;
			if ((1 + movement) % m_board_size == 1) touch_left = true;
		}

		if (touch_left == true && touch_right == true)
		{
			m_shut_down = true;
			return PLAYER;
		}
	}
	return NOBODY;
}

PLAYERS Board::IsWinnerComputer()
{
	bool touch_up = false;
	bool touch_down = false;
	int movement = 0;

	m_hex_to_check = m_num_nodes + VIRTUAL_NODE::NORTH_NODE; //NORTH VIRTUAL NODE
	SearchNodeTree(COMPUTER);

	for (auto tree_b = m_tree.begin(), tree_e = m_tree.end(); tree_b != tree_e; ++tree_b)
	{
		movement = *tree_b;

		if (movement < m_num_nodes) //If it's not a virtual node
		{

			if (m_hex_board[movement] == COMPUTER)
			{
				if (movement / m_board_size == 0) touch_up = true;
				if (movement / m_board_size == m_board_size - 1) touch_down = true;
			}
		}

		if (touch_up == true && touch_down == true)
		{
			m_tree.clear();
			return COMPUTER;
		}
		
	}
	m_tree.clear();
	return NOBODY; //Or player
}


// A MODIFICATION ON THE PRIM ALGORITHM THAT DOESN'T CONSIDER THE COSTS
void Board::SearchNodeTree(PLAYERS player)
{
    int direction = 0, row = 0, column = 0;
    std::vector<bool> visited_nodes(m_num_nodes + 4);
    nodes_to_visit.push_back(m_hex_to_check);
    m_tree.push_back(m_hex_to_check);

    while(!nodes_to_visit.empty())
    {
		adjacent_edges = m_graph_board.GetEdges(*(nodes_to_visit.end() - 1));
        nodes_to_visit.pop_back();

        //For every node to visit, check all is edges if are not checked before and add it to the posible edges to check.
        for(auto adj_b = adjacent_edges.begin(),  adj_e = adjacent_edges.end(); adj_b != adj_e; ++adj_b )
        {
            direction = adj_b->m_direction;


            //If it's not a virtual node then check normally
            if(direction < m_num_nodes )
            {

                if( visited_nodes[direction] == false)
				if (m_hex_board[direction] == player) 
                {
					posible_edges.push_back(direction);
                }
            }

            //If it's a virtual node then check if it's from the current player
            else
            {
                if(visited_nodes[direction] == false && player == COMPUTER && (direction == m_num_nodes + VIRTUAL_NODE::NORTH_NODE || direction == m_num_nodes + VIRTUAL_NODE::SOUTH_NODE ))
                {
                    posible_edges.push_back(direction);
                }

                else if(visited_nodes[direction] == false  && player == PLAYER  &&
                       (direction == m_num_nodes + VIRTUAL_NODE::WEST_NODE || direction == m_num_nodes + VIRTUAL_NODE::EAST_NODE))
                {
					posible_edges.push_back(direction);
                }
            }
        }

        //Check the posible nodes to visit and add it to the tree if the id it's not visited
        while(!posible_edges.empty())
        {
            int hex = *(posible_edges.end()-1);

            if( visited_nodes[hex] == false)
            {
                visited_nodes[hex] = true;
                nodes_to_visit.push_back(hex);
                m_tree.push_back(hex);
            }
            posible_edges.pop_back();
        }
    }
}
