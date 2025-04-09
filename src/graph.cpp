#include "headers/graph.h"
#include "headers/random_generator.h"
#include <iostream>
#include <cmath>

void Graph::create_random_graph(size_t elements, double probability)
{
    ids.resize(elements);
    std::iota(ids.begin(), ids.end(), 0);
    graph.clear();
    this->elements = elements;
    this->probability = probability;
    for (size_t id = 0; id != elements; id++)
    {
        vertex cur_vertex(elements, false);             // создаем вершину графа
        for (size_t n_id = 0; n_id != elements; n_id++) // цикл по соседям
        {
            if (should_generate(probability)) // добавлять ли соседа
            {
                cur_vertex[n_id] = true;
            }
        }
        add_vertex(cur_vertex); // добавляем вершину с соседями в граф
    }
}

Graph::Graph(std::vector<vertex> graph, size_t elements) : graph(graph), elements(elements)
{
    ids.resize(elements);
    std::iota(ids.begin(), ids.end(), 0);
}

void Graph::remove_vertex(size_t id)
{
    for (size_t i = 0; i != elements; i++)
    {
        graph[i].erase(graph[i].begin() + id);
    }
    graph.erase(graph.begin() + id);
    ids.erase(ids.begin() + id);
    elements--;
}

void Graph::print_matrix()
{
    for (size_t i = 0; i != elements; i++)
    {
        for (size_t j = 0; j != elements; j++)
        {
            if (graph[i][j])
            {
                std::cout << "1" << " ";
            }
            else
            {
                std::cout << "0" << " ";
            }
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