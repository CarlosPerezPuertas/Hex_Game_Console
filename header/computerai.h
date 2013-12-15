#ifndef COMPUTERAI_H
#define COMPUTERAI_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include "Graph.h"
#include "Montecarlo.h"
#include <concurrent_vector.h>


class Board;

class ComputerAI
{
    public:
        ComputerAI();
        virtual ~ComputerAI();

        int BestMovement(Board *board);
        
};

#endif // COMPUTERAI_H
