#include <nlohmann/json.hpp>
#include "graph.h"
#include "graph_algorithms.h"
#include "random_generator.h"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;
class graph_tester
{
    json test_full_karger;
    json test_random_karger;

    json test_cyclomatic;
    json test_components;

    json test_diameter;
    int runs = 100;
    double prob_start = 0.05;

    double prob_step = 0.01;
    double k_step = 0.01;
    int karger_runs = 100;
    std::vector<int> graph_sizes = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    int rand_karg_size = 50;
    int rand_karg_iters = 10;

    std::string dir = "../src/analysis";

public:
    void
    karger_test_full_graph()
    {
        test_full_karger["karger_runs"] = karger_runs;
        test_full_karger["k_step"] = k_step;
        for (int size : {10, 20, 30})
        {
            std::vector<double> probabilities;
            Graph test_graph;
            test_graph.create_random_graph(size - 1, 1);
            vertex new_vertex(test_graph.elements - 1, 0);
            std::iota(new_vertex.begin(), new_vertex.end(), 0);
            test_graph.add_vertex(new_vertex);
            int elements_sq = test_graph.elements * test_graph.elements;
            for (int k = 1; k < elements_sq; k += 5)
            {
                int iterations = k;
                int counter = 0;
                for (int i = 0; i != karger_runs; i++)
                {
                    int result = karger_algorithm(test_graph, iterations);
                    if (result == test_graph.elements - 2)
                    {
                        counter++;
                    }
                }
                double prob = (double)counter / (double)karger_runs;
                double teor_prob = (double)1 - (double)pow((1 - (double)2 / (test_graph.elements * (test_graph.elements - 1))), k);
                std::cout << "prob: " << prob << " teor_prob: " << teor_prob << " iters: " << k << " size: " << test_graph.elements << "\n";
                probabilities.push_back(prob);
            }
            test_full_karger[std::to_string(size)] = probabilities;
        }
        save_test(test_full_karger, dir + "/test_full_karger.json");
    }

    void karger_test_random()
    {
        test_random_karger["prob_step"] = prob_step;
        test_random_karger["prob_start"] = 0.1;
        test_random_karger["graph_size"] = rand_karg_size;
        test_random_karger["karger_iters"] = rand_karg_iters;
        std::vector<double> probabilities;
        for (double prob = 0.1; prob < 1.01; prob += prob_step)
        {

            Graph test_graph;
            while (connected_components(test_graph) != 1)
            {
                test_graph.create_random_graph(rand_karg_size - 1, prob);
            }
            test_graph.add_vertex({randrange(0, rand_karg_size - 2)});

            int counter = 0;
            for (int i = 0; i != karger_runs; i++)
            {
                int result = karger_algorithm(test_graph, rand_karg_iters);
                if (result == 1)
                {
                    counter++;
                }
            }
            double karger_prob = (double)counter / (double)karger_runs;
            double teor_prob = (double)1 - (double)pow((1 - (double)2 / (test_graph.elements * (test_graph.elements - 1))), rand_karg_iters);
            std::cout << "graph_prob: " << prob << " karger_prob: " << karger_prob << " teor_karger_prob: " << teor_prob << "\n";
            probabilities.push_back(karger_prob);
        }
        test_random_karger["karger_probabilities"] = probabilities;
        save_test(test_random_karger, dir + "/test_random_karger.json");
    }

    void cyclomatic_test()
    {
        test_cyclomatic["prob_step"] = prob_step;
        test_cyclomatic["prob_start"] = prob_start;
        test_cyclomatic["sizes"] = graph_sizes;
        for (int size : graph_sizes)
        {
            std::cout << "start size: " << size << "\n";
            Graph test_graph;
            std::vector<int> cyclomatic_numbers;
            for (double prob = prob_start; prob < 1.01; prob += prob_step)
            {
                int counter = 0;
                for (int i = 0; i != runs; i++)
                {
                    test_graph.create_random_graph(size, prob);
                    counter += cyclomatic_number(test_graph);
                }
                int mean_cyclomatic = counter / runs;
                cyclomatic_numbers.push_back(mean_cyclomatic);
            }
            test_cyclomatic[std::to_string(size)] = cyclomatic_numbers;
        }
        save_test(test_cyclomatic, dir + "/test_cyclomatic.json");
    }

    void component_test()
    {
        test_components["prob_step"] = prob_step;
        test_components["prob_start"] = prob_start;
        test_components["sizes"] = graph_sizes;
        for (int size : graph_sizes)
        {
            std::cout << "start size: " << size << "\n";
            Graph test_graph;
            std::vector<int> component_sizes;
            for (double prob = prob_start; prob < 1.01; prob += prob_step)
            {
                int counter = 0;
                for (int i = 0; i != runs; i++)
                {
                    test_graph.create_random_graph(size, prob);
                    counter += connected_components(test_graph);
                }
                int mean_components = counter / runs;
                component_sizes.push_back(mean_components);
            }
            test_components[std::to_string(size)] = component_sizes;
        }
        save_test(test_components, dir + "/test_components.json");
    }

    void diameter_test()
    {
        test_diameter["prob_start"] = prob_start;
        test_diameter["prob_step"] = prob_step;
        test_diameter["sizes"] = graph_sizes;
        for (int size : graph_sizes)
        {
            std::cout << "start size: " << size << "\n";
            Graph test_graph;
            std::vector<int> diameter_sizes;
            for (double prob = prob_start; prob < 1.01; prob += prob_step)
            {
                while (connected_components(test_graph) != 1)
                {
                    test_graph.create_random_graph(rand_karg_size - 1, prob);
                }
                int counter = 0;
                for (int i = 0; i != runs; i++)
                {
                    test_graph.create_random_graph(size, prob);
                    counter += diameter_size(test_graph);
                }
                int mean_diameter = counter / runs;
                diameter_sizes.push_back(mean_diameter);
            }
            test_diameter[std::to_string(size)] = diameter_sizes;
        }
        save_test(test_diameter, dir + "/test_diameter.json");
    }

    void save_test(const json &test, std::string fname)
    {
        std::ofstream file(fname);
        if (file.is_open())
        {
            file << test.dump(4);
            file.close();
            std::cout << "JSON saved to" << fname << std::endl;
        }
        else
        {
            std::cerr << "Failed to open file for writing." << std::endl;
        }
    }
};
