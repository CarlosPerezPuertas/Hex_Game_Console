#include "computerai.h"
#include "Board.h"


ComputerAI::ComputerAI()
{

}

ComputerAI::~ComputerAI()
{

}



//MAKE A MOVEMENT AND MAKE A SIMULATION TO KNOW THE PROBABILITY OF WIN OF EACH MOVEMENT
//WE CHOOSE THE MOVEMENT WITH THE HIGHEST WINNING PROBABILITY
int ComputerAI::BestMovement(Board *board)
{
    std::chrono::time_point<std::chrono::system_clock> start_point, end_point;
    start_point = std::chrono::system_clock::now();

    double win_probability = 0.0;
    double best_win_probability = 0.0;
    int BEST_MOVEMENT = 0;
    int board_size = board->BoardSize();

	std::vector<PLAYERS> hex_board = board->HexBoard();
    int i = 0;

	concurrency::concurrent_vector<std::future<double>> futures;
	futures.reserve(board_size*board_size);
	std::vector<int> movements;
	
	Montecarlo MS; //montecarlo object to run in the thread

	int number_of_threads = 0;
	const int MAX_THREADS = std::thread::hardware_concurrency(); //The number of threads supported by the hardware

    for(auto board_b = hex_board.begin(), board_e = hex_board.end(); board_b != board_e; ++board_b, ++i)
    {
		if (*board_b == NOBODY)
        {
			number_of_threads++;
			futures.push_back(std::async(std::launch::async, &Montecarlo::MontecarloSimulation, MS, *board, i));
			movements.push_back(i); 

		    if (number_of_threads == MAX_THREADS) //We have a limit of threads supported by the hardware
			{
				number_of_threads = 0;
					
				for (unsigned int j = 0; j < futures.size(); j++)
				{
					double probability = futures[j].get();

					if (probability > best_win_probability)
					{
						best_win_probability = probability;
						BEST_MOVEMENT = movements[j];
						//std::cout << "1BEST_MOVEMENT: " << BEST_MOVEMENT << "  best_win_probability: " << best_win_probability << std::endl;
					}
				}
				futures.clear();	
				movements.clear();
			}
		}
    }

	//We iterate through the last futures that have not been evaluated before
	for (unsigned int i = 0; i < futures.size(); i++)
	{
		double probability = futures[i].get();

		if (probability > best_win_probability)
		{
			best_win_probability = probability;
			BEST_MOVEMENT = movements[i];
			std::cout << "2BEST_MOVEMENT: " << BEST_MOVEMENT << "  best_win_probability: " << best_win_probability << std::endl;
		}
	}

    end_point = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_point - start_point;
    std::cout << "Time elapsed: " << elapsed_seconds.count() << "s" <<std::endl;

    return BEST_MOVEMENT;
}


