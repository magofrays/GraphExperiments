#include "headers/graph_algorithms.h"

void pull_together(Graph &graph, int first, int second)
{
    for (size_t i = 0; i != graph.elements; i++)
    {
        graph[first][i] = graph[first][i] || graph[second][i];
        graph[i][first] = graph[i][first] || graph[i][second];
    }
    graph.remove_vertex(second);
}

Graph karger_algorithm(const Graph &graph)
{
    std::vector<std::pair<int, int>> edges;
    edges.reserve(graph.elements * graph.elements);
    Graph temp_graph = graph;
    while (temp_graph.elements > 2)
    {
        edges.clear();
        int index = 0;
        for (int i = 0; i < temp_graph.elements; i++)
        {
            for (int j = i + 1; j < temp_graph.elements; j++)
            {
                if (temp_graph[i][j])
                {
                    edges[index] = {i, j};
                    index++;
                }
            }
        }
        auto edge = random_choice(edges);
        pull_together(temp_graph, edge->first, edge->second);
    }
    return temp_graph;
}