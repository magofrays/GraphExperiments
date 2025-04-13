#include <iostream>
#include "graph_algorithms.h"
#include "visualizer.h"
#include "graph_tester.cpp"

int main(int argc, char **argv)
{
    graph_tester g_test;
    g_test.diameter_test();
    return 0;
}