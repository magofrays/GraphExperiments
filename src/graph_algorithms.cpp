#include "graph_algorithms.h"
#include <iostream>

void pull_together(Graph &graph, int first, int second) //
{
    for (size_t i = 0; i != graph.elements; i++)
    {
        graph[first][i] += graph[second][i];
        graph[i][first] += graph[i][second];
    }
    graph.remove_vertex(second);
    for (size_t i = 0; i != graph.elements; i++)
    {
        graph[i][i] = 0;
    }
}

int karger_algorithm_iteration(const Graph &graph)
{
    std::vector<std::pair<int, int>> edges;
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
                    edges.push_back({i, j});
                    index++;
                }
            }
        }
        auto edge = random_choice(edges);
        pull_together(temp_graph, edge->first, edge->second);
    }
    return temp_graph[0][1];
}

int karger_algorithm(const Graph &graph, int iterations)
{
    int min_cut = karger_algorithm_iteration(graph);
    for (int i = 0; i < iterations - 1; i++)
    {
        int cut = karger_algorithm_iteration(graph);
        if (cut == 0)
        {
            std::cout << "error! cut = 0\n";
        }
        if (cut < min_cut)
        {
            min_cut = cut;
        }
    }
    return min_cut;
}

std::unordered_set<int> BFS(const Graph &graph, int start_index)
{
    std::unordered_set<int> visited;
    std::queue<int> to_visit;
    to_visit.push(start_index);
    while (!to_visit.empty())
    {
        int node_index = to_visit.front();
        to_visit.pop();
        if (!visited.count(node_index))
        {
            visited.insert(node_index);
            for (int neigbor : graph.neighbors(node_index))
            {
                if (!visited.count(neigbor))
                {
                    to_visit.push(neigbor);
                }
            }
        }
    }
    return visited;
}

int connected_components(const Graph &graph)
{
    if (graph.elements == 0)
    {
        return 0;
    }
    std::unordered_set<int> nodes;
    for (int i = 0; i != graph.elements; i++)
    {
        nodes.insert(i);
    }
    int components = 0;
    while (!nodes.empty())
    {
        auto visited_nodes = BFS(graph, *nodes.begin());
        for (int node : visited_nodes)
        {
            nodes.erase(node);
        }
        components++;
    }
    return components;
}

int cyclomatic_number(const Graph &graph)
{
    return graph.edges - graph.elements + connected_components(graph);
}

std::pair<int, int> get_longest_path_DFS(const Graph &graph, int start_index)
{
    std::vector<int> visited(graph.elements, 0);
    std::vector<int> path_length(graph.elements, 0);
    std::stack<int> to_visit;
    to_visit.push(start_index);
    while (!to_visit.empty())
    {
        int cur_index = to_visit.top();
        to_visit.pop();
        if (!visited[cur_index])
        {
            visited[cur_index] = 1;
            for (int neigbor : graph.neighbors(cur_index))
            {
                if (visited[neigbor] == 0)
                {
                    to_visit.push(neigbor);
                    path_length[neigbor] = path_length[cur_index] + 1;
                }
            }
        }
    }
    int max_index = 0;
    int max_length = path_length[0];
    for (int i = 1; i != graph.elements; i++)
    {
        if (path_length[i] > max_length)
        {
            max_index = i;
            max_length = path_length[i];
        }
    }
    return {max_index, max_length};
}

int diameter_size(const Graph &graph)
{
    auto longest_path_from_0 = get_longest_path_DFS(graph, 0);
    auto longest_path = get_longest_path_DFS(graph, longest_path_from_0.first);
    return longest_path.second;
}
