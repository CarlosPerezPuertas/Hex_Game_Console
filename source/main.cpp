#include <ctime>
#include "Board.h"


int main()
{
	srand((unsigned int)time(0));

	Board board;
	board.Run();

	return 0;
}
