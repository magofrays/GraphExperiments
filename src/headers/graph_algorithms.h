#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "graph.h"
#include "headers/random_generator.h"

void pull_together(Graph &graph, int first, int second);
Graph karger_algorithm(const Graph &graph);
Graph karger_shtern_algorithm();

#endif