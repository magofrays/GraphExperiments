#ifndef GRAPH_H
#define GRAPH_H
#include <vector>

using vertex = std::vector<bool>;

struct Graph
{
    size_t elements;                            // number of nodes
    double probability;                         // probability for generation
    std::vector<vertex> graph;                  // vector of nodes
    std::vector<std::pair<int, int>> positions; // nodes positions
    Graph() = default;

    void add_vertex(vertex &v) { graph.push_back(v); }
    void remove_vertex(size_t id);
    void create_random_graph(size_t elements, double probability);
    void set_standart_positions();

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