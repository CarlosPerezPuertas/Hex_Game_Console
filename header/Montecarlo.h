#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include "Graph.h"

const int NUMBER_OF_SIMULATIONS = 1000;

class Board;

class Montecarlo
{
public:
	Montecarlo();
	virtual ~Montecarlo();

	double Montecarlo::MontecarloSimulation(Board &board, int movement);
};

#endif

