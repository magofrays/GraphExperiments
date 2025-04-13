#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <numeric>
#include "random_generator.h"

using vertex = std::vector<int>;

struct Graph
{
    size_t elements = 0;                              // number of nodes
    int edges = 0;                                    // number of edges
    double probability;                               // probability for generation
    bool orientated;                                  // is graph orientated
    std::vector<vertex> graph;                        // vector of nodes
    std::vector<std::pair<double, double>> positions; // nodes positions
    std::vector<size_t> ids;                          // node ids
    Graph() = default;
    Graph(std::vector<vertex> graph, size_t elements, bool orientated = false);

    void add_vertex(std::vector<int> neighbors_indexes);
    void remove_vertex(size_t id);
    void create_random_graph_orientated(size_t elements, double probability);
    void create_random_graph(size_t elements, double probability);
    void set_positions_on_square();
    void set_positions_around_circle();
    int count_edges();

    std::vector<int> neighbors(int node_index) const; // returns indexes

    void print_matrix();
    vertex &operator[](size_t index)
    {
        return graph[index];
    }
    const vertex &operator[](size_t index) const
    {
        return graph[index];
    }
};
#endif