#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "graph.h"
#include "random_generator.h"
#include <unordered_set>
#include <stack>
#include <queue>

void pull_together(Graph &graph, int first, int second);
int karger_algorithm_iteration(const Graph &graph);
int karger_algorithm(const Graph &graph, int iterations);
std::unordered_set<int> BFS(const Graph &graph, int start_index);
int connected_components(const Graph &graph);
int cyclomatic_number(const Graph &graph);
std::pair<int, int> get_longest_path_DFS(const Graph &graph, int start_index);
int diameter_size(const Graph &graph);

#endif