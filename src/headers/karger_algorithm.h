#ifndef KARGER_H
#define KARGER_H

#include "headers/graph.h"

Graph pull_together(const Graph &graph, int first, int second);
Graph karger_iteration();
Graph karger_algorithm();
Graph karger_shtern_algorithm();

#endif