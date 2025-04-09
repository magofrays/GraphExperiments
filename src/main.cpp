#include <iostream>
#include "headers/karger_algorithm.h"
#include "headers/visualizer.h"

int main(int argc, char **argv)
{
    Visualizer *gui = Visualizer::instance(argc, argv);

    Graph graph;
    graph.create_random_graph(9, 0.5);
    graph.set_standart_positions();

    gui->draw_graph(graph);
    gui->show();
    return gui->exec();
}