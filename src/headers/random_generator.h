#include <random>

static std::random_device rd;
static std::mt19937 generator(rd());
static std::uniform_real_distribution<double> distribution(0.0, 1.0);

bool should_generate(double probability) { return distribution(generator) <= probability ? true : false; }
