#include <vector>

using vertex = std::vector<bool>;

struct Graph
{
    size_t elements;           // number of nodes
    double probability;        // probability for generation
    std::vector<vertex> graph; // list of nodes
    Graph() = default;

    void add_vertex(vertex &v) { graph.push_back(v); }

    void create_random_graph(size_t elements, double probability);

    void print_matrix();
};