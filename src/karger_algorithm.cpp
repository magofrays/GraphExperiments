#include "headers/karger_algorithm.h"

Graph pull_together(const Graph &graph, int first, int second)
{
    Graph new_graph(graph);
    for (size_t i = 0; i != new_graph.elements; i++)
    {
        new_graph[first][i] = graph[first][i] || graph[second][i];
        new_graph[i][first] = graph[i][first] || graph[i][second];
    }
    new_graph.remove_vertex(second);
    for (size_t i = 0; i != new_graph.elements; i++)
    {
        new_graph[i][i] = 0;
    }
    return new_graph;
}