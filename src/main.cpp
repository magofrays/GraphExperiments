#include <iostream>
#include "headers/graph_algorithms.h"
#include "headers/visualizer.h"

int main(int argc, char **argv)
{
    Visualizer *gui = Visualizer::instance(argc, argv);

    Graph graph({{false, true, true, false},
                 {false, false, true, true},
                 {false, false, false, true},
                 {false, false, false, true}},
                4);

    graph.create_random_graph(100, 1);
    graph.set_positions_around_circle();
    gui->draw_graph(graph, 0, 0);
    // auto new_graph = karger_algorithm(graph);
    // pull_together(graph, 1, 2);
    // gui->draw_graph(graph, 600, 0);
    gui->show();
    return gui->exec();
}