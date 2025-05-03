#include "graph.h"
#include <iostream>
#include <cmath>

void Graph::create_random_graph(size_t elements, double probability)
{
    ids.resize(elements);
    std::iota(ids.begin(), ids.end(), 0);
    graph.clear();
    orientated = false;
    graph = std::vector<vertex>(elements, vertex(elements, 0));
    this->elements = elements;
    this->probability = probability;
    for (size_t id = 0; id != elements; id++)
    {
        for (size_t n_id = id + 1; n_id != elements; n_id++) // цикл по соседям
        {
            if (should_generate(probability)) // добавлять ли соседа
            {
                graph[id][n_id] = 1;
                graph[n_id][id] = 1;
            }
        }
    }
    edges = count_edges();
}

int Graph::count_edges()
{
    int edge_counter = 0;
    for (int y = 0; y < elements; y++)
    {
        for (int x = orientated ? 0 : y + 1; x < elements; x++)
        {
            if (graph[y][x])
            {
                edge_counter++;
            }
        }
    }
    return edge_counter;
}

void Graph::create_random_graph_orientated(size_t elements, double probability)
{
    ids.resize(elements);
    std::iota(ids.begin(), ids.end(), 0);
    graph.clear();
    graph = std::vector<vertex>(elements, vertex(elements, 0));
    orientated = true;
    this->elements = elements;
    this->probability = probability;
    for (size_t id = 0; id != elements; id++)
    {
        for (size_t n_id = 0; n_id < elements; n_id++) // цикл по соседям
        {
            if (should_generate(probability)) // добавлять ли соседа
            {
                graph[id][n_id] = 1;
            }
        }
    }
    edges = count_edges();
}

Graph::Graph(std::vector<vertex> graph, size_t elements, bool orientated) : graph(graph), elements(elements), orientated(orientated)
{
    ids.resize(elements);
    std::iota(ids.begin(), ids.end(), 0);
    edges = count_edges();
}

void Graph::add_vertex(std::vector<int> neighbors_indexes)
{
    vertex new_vertex(elements + 1, 0);
    for (auto &v : graph)
    {
        v.push_back(0);
    }
    for (int index : neighbors_indexes)
    {
        new_vertex[index] = 1;
        if (!orientated)
            graph[index][elements] = 1;
    }
    elements++;
    graph.push_back(new_vertex);
    ids.push_back(elements - 1);
    edges = count_edges();
}

void Graph::remove_vertex(size_t id)
{
    for (size_t i = 0; i != elements; i++)
    {
        graph[i].erase(graph[i].begin() + id);
    }
    graph.erase(graph.begin() + id);
    ids.erase(ids.begin() + id);
    positions.erase(positions.begin() + id);
    edges = count_edges();
    elements--;
}

void Graph::print_matrix()
{
    for (size_t i = 0; i != elements; i++)
    {
        for (size_t j = 0; j != elements; j++)
        {
            std::cout << graph[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Graph::set_positions_on_square()
{
    int row_size = std::sqrt(elements);
    positions = std::vector<std::pair<double, double>>(elements);
    int y = 1;
    for (int i = 0; i != elements; i++)
    {
        int x = (i % row_size);
        if (i % row_size == 0)
        {
            y++;
        }
        positions[i] = {x, y};
    }
}

void Graph::set_positions_around_circle()
{
    positions = std::vector<std::pair<double, double>>(elements);
    for (int i = 0; i != elements; i++)
    {
        double k = i * M_PI * 2 / elements;
        positions[i] = {std::sin(k), std::cos(k)};
    }
}

std::vector<int> Graph::neighbors(int node_index) const
{
    std::vector<int> neighbors;
    for (int i = 0; i != elements; i++)
    {
        if (graph[node_index][i])
        {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}
