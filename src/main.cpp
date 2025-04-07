#include "headers/graph.h"

int main(int argc, char const *argv[])
{
    Graph graph;
    graph.create_random_graph(10, 0.5);
    graph.print_matrix();
    graph.remove_vertex(5); // fix
    graph.print_matrix();
    return 0;
}
