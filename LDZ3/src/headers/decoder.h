#ifndef DECODER_H
#define DECODER_H

#include <unordered_set>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <stack>
#include <functional>
#include "aho_corasick.h"
#include <cmath>
#include <sstream>

struct func_container
{
    std::string name;
    bool two_var_flag;
    std::function<double(double, double)> func;
};

class Decoder
{
    double zero_limit = 0.001;

    std::unordered_set<std::string>
        zero{"(", ")"},
        first{"-", "+"},
        second{"*", "/"},
        third{"^"},
        fourth{"sin", "cos", "tg", "ctg", "exp", "ln", "arcsin", "arccos", "arctg", "arcctg", "sqrt", "~"};

    std::vector<std::string> words_to_split = {
        "sin", "cos", "tg", "ctg", "sqrt", "exp", "ln",
        "arcsin", "arccos", "arcctg", "arctg", "-", "+",
        "*", "/", "(", ")", "^", "x", "~"};
    bohr automation;
    static Decoder *instance_ptr;

public:
    std::unordered_map<std::string, func_container> func_map;
    Decoder(const Decoder &) = delete;
    void operator=(const Decoder &) = delete;

    static Decoder *get()
    {
        if (instance_ptr == nullptr)
        {
            instance_ptr = new Decoder();
        }

        return instance_ptr;
    }

    Decoder();
    std::vector<std::string> parse(std::string &text);
    void remove_other(std::stack<std::pair<std::string, int>> &priority_stack, std::vector<std::string> &result, int priority);
    std::vector<std::string> postfix(std::vector<std::string> &parsed_elements);
    std::vector<std::string> simplify_postfix(std::vector<std::string> &postfix);

    friend class syntaxTree;
    friend class syntaxNode;
};

bool to_double(std::string &element, double &num);

std::string double_to_string(double number, int precision);

#endif