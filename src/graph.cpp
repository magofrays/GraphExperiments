#include "headers/graph.h"
#include "headers/random_generator.h"
#include <iostream>

void Graph::create_random_graph(size_t elements, double probability)
{
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
