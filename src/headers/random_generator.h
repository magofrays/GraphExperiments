#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <vector>

static std::mt19937 generator(std::random_device{}());

inline bool should_generate(double probability)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator) <= probability;
}

template <typename T>
inline typename std::vector<T>::iterator random_choice(std::vector<T> &elements)
{
    if (elements.empty())
    {
        return elements.end();
    }
    std::uniform_int_distribution<int> dist(0, elements.size() - 1);
    return elements.begin() + dist(generator);
}

inline int randrange(int start, int end)
{
    std::uniform_int_distribution<int> dist(start, end);
    return dist(generator);
}

#endif