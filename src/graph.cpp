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
        vertex cur_vertex(id, elements);
        cur_vertex.id = id;
        for (size_t n_id = 0; n_id != elements; n_id++)
        {
            if (should_generate(probability))
            {
                cur_vertex.neightbors[n_id] = true;
            }
        }
        add_vertex(cur_vertex);
    }
}

void Graph::print_matrix()
{
    for (size_t i = 0; i != elements; i++)
    {
        for (size_t j = 0; j != elements; j++)
        {
            if (graph[i].neightbors[j])
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
