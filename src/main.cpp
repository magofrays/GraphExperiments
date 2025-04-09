#include <iostream>
#include "headers/karger_algorithm.h"
#include "headers/visualizer.h"

int main(int argc, char **argv)
{
    Visualizer *gui = Visualizer::instance(argc, argv);

    Graph graph({{false, true, true, false},
                 {false, false, true, true},
                 {false, false, false, true},
                 {false, false, false, true}},
                4);
    // graph.create_random_graph(4, 1);
    graph.set_positions_on_square();
    gui->draw_graph(graph);
    auto new_graph = pull_together(graph, 1, 2);

    // gui->draw_graph(graph, 200, 100);
    gui->draw_graph(new_graph, 200, 200);
    gui->show();
    return gui->exec();
}