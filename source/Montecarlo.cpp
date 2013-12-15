#include "Montecarlo.h"
#include "board.h"


Montecarlo::Montecarlo()
{
}


Montecarlo::~Montecarlo()
{
}


//MAKE A MOVEMENT AND FILL THE BOARD TO KNOW THE PROBABILITIES OF WINNING OF EACH MOVEMENT USING 1000 ALEATORY SIMULATIONS
double Montecarlo::MontecarloSimulation(Board &board, int movement)
{
	int computer_wins = 0;
	int board_size = board.BoardSize();

	Board board_copy(board);
	board_copy.MakeComputerMovement(movement);

	std::vector<int> empty_hex_restart = board_copy.EmptyHex();

	if (board_copy.IsWinnerComputer() == NOBODY)
	{
		for (int m = 0; m < NUMBER_OF_SIMULATIONS; m++)
		{
			board_copy.MakeRandomMovement();            //Player makes movement then changes to computer and so on
			
			int winner_player = board_copy.IsWinnerComputer();
			if (winner_player == COMPUTER) computer_wins++;

			board_copy.SetEmptyHex(empty_hex_restart);  //Rollback the board
		}
	}

	else return 1.0;                                    //Winning movement for the computer

	return computer_wins / static_cast<double>(NUMBER_OF_SIMULATIONS);
}
