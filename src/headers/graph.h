#include <vector>

struct vertex
{
    size_t id;
    std::vector<bool> neightbors; // vector true/false
    vertex(size_t id, std::vector<bool> neightbors) : id(id), neightbors(neightbors) {}
    vertex(size_t id, size_t n_size)
    {
        this->id = id;
        neightbors = std::vector<bool>(n_size, false);
    }
};

struct Graph
{
    size_t elements;           // number of nodes
    double probability;        // probability for generation
    std::vector<vertex> graph; // list of nodes
    Graph() = default;

    void add_vertex(size_t id, std::vector<bool> neightbors) { graph.push_back({id, neightbors}); }

    void add_vertex(vertex &v) { graph.push_back(v); }

    void create_random_graph(size_t elements, double probability);

    void print_matrix();
};