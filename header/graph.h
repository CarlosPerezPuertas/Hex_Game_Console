#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <thread>

//Value of virtual nodes used to check if there is a winner with the modified prim algorithm
enum VIRTUAL_NODE{NORTH_NODE = 0, EAST_NODE, SOUTH_NODE, WEST_NODE };

struct Edge
{
    int m_origin;
    int m_direction;

    Edge(int origin, int direction) : m_origin(origin), m_direction(direction)
    {

    }

    void Reverse()
    {
        int temp = 0;
        temp = m_origin;
        m_origin = m_direction;
        m_direction = temp;
    }
};

typedef std::vector<Edge> Edges;
typedef std::vector<Edges> NodeAndEdges;


class Graph
{
    private:
        int m_num_nodes;
        NodeAndEdges m_graph;

    public:
		Graph();
        Graph(int board_size);
        virtual ~Graph();

        void AddEdge(int origin, int direction);
        inline int GetNumNodes() {return m_num_nodes;}
        inline const Edges& GetEdges (int node) const {return m_graph[node];}	
};

#endif // GRAPH_H
