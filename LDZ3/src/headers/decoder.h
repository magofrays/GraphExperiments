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

bool is_digits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

class Decoder
{
    struct func
    {
        std::string name;
        bool two_var_flag;
        std::function<double(double, double)> func;
    };
    double zero_limit = 0.001;
    std::unordered_map<std::string, func> func_map;
    std::unordered_set<std::string> first = {"-", "+"};
    std::unordered_set<std::string> second = {"*", "/"};
    std::unordered_set<std::string> third = {"^"};
    std::unordered_set<std::string> fourth = {"sin", "cos", "tg", "ctg", "exp", "ln", "arcsin", "arccos", "arctg", "arcctg", "sqrt"};
    std::unordered_set<std::string> zero = {"(", ")"};
    std::vector<std::string> words_to_split = {
        "sin", "cos", "tg", "ctg", "sqrt", "exp",
        "ln", "arcsin", "arccos", "arcctg", "arctg",
        "-", "+", "*", "/", "(", ")", "^", "x"};
    bohr automation;

public:
    Decoder()
    {
        for (auto &word : words_to_split)
        {
            automation.add_string(word);
        }
        func_map["sin"] = {"sin", false, [](double x, double plug)
                           { return std::sin(x); }};
        func_map["cos"] = {"cos", false, [](double x, double plug)
                           { return std::cos(x); }};
        func_map["tg"] = {"tg", false, [](double x, double plug)
                          { return std::tan(x); }};
        func_map["ctg"] = {"ctg", false, [](double x, double plug)
                           { return 1 / std::tan(x); }};
        func_map["exp"] = {"exp", false, [](double x, double plug)
                           { return std::exp(x); }};
        func_map["ln"] = {"ln", false, [](double x, double plug)
                          { return std::log(x); }};
        func_map["arcsin"] = {"arcsin", false, [](double x, double plug)
                              { return std::asin(x); }};
        func_map["arccos"] = {"arccos", false, [](double x, double plug)
                              { return std::acos(x); }};
        func_map["arctg"] = {"arctg", false, [](double x, double plug)
                             { return std::atan(x); }};
        func_map["arcctg"] = {"arcctg", false, [](double x, double plug)
                              { return M_PI / 2 - std::atan(x); }};
        func_map["sqrt"] = {"sqrt", false, [](double x, double plug)
                            { return std::sqrt(x); }};
        func_map["+"] = {"+", true, [](double x, double y)
                         { return x + y; }};
        func_map["-"] = {"-", true, [](double x, double y)
                         { return x - y; }};
        func_map["*"] = {"*", true, [](double x, double y)
                         { return x * y; }};
        func_map["/"] = {"/", true, [](double x, double y)
                         { return x / y; }};
        func_map["^"] = {"^", true, [](double x, double y)
                         { return std::pow(x, y); }};
        func_map["x"] = {"x", false, [](double x, double y)
                         { return x; }};
    }

    std::vector<std::string> parse(std::string &text)
    {
        auto positions = automation.find_all_pos(text);
        std::sort(positions.begin(), positions.end());
        for (int i = positions.size() - 1; i > 0; i--)
        {
            if (positions[i].first < positions[i - 1].first + positions[i - 1].second)
            {
                positions.erase(positions.begin() + i);
            }
        }
        std::vector<std::string> parsed_elements;
        for (auto &pos : positions)
        {
            parsed_elements.push_back(text.substr(pos.first, pos.second));
        }
        return parsed_elements;
    }

    void remove_other(std::stack<std::pair<std::string, int>> &priority_stack, std::vector<std::string> &result, int priority)
    {
        while (!priority_stack.empty() && priority_stack.top().second >= priority)
        {
            if (priority == 0 && priority_stack.top().second == 0)
            {
                priority_stack.pop();
                return;
            }
            result.push_back(priority_stack.top().first);
            priority_stack.pop();
        }
    }

    std::vector<std::string> postfix(std::vector<std::string> &parsed_elements)
    {
        std::stack<std::pair<std::string, int>> priority_stack;
        std::vector<std::string> result;
        for (auto &element : parsed_elements)
        {
            if (first.find(element) != first.end())
            {
                remove_other(priority_stack, result, 1);
                priority_stack.push({element, 1});
            }
            else if (second.find(element) != second.end())
            {
                remove_other(priority_stack, result, 2);
                priority_stack.push({element, 2});
            }
            else if (third.find(element) != third.end())
            {
                remove_other(priority_stack, result, 3);
                priority_stack.push({element, 3});
            }
            else if (fourth.find(element) != fourth.end())
            {
                remove_other(priority_stack, result, 4);
                priority_stack.push({element, 4});
            }
            else if (zero.find(element) != zero.end())
            {
                if (element == "(")
                {
                    priority_stack.push({element, 0});
                }
                else
                {
                    remove_other(priority_stack, result, 0);
                }
            }
            else
            {

                result.push_back(element);
            }
        }
        while (!priority_stack.empty())
        {
            result.push_back(priority_stack.top().first);
            priority_stack.pop();
        }
        return result;
    }
    bool to_double(std::string &element, double &num)
    {
        std::istringstream i(element);
        if (i >> num)
        {
            return true;
        }
        return false;
    }

    std::string double_to_string(double number, int precision)
    {
        std::ostringstream stream;
        stream.precision(precision);
        stream << std::fixed << number;
        return stream.str();
    }

    std::vector<std::string> simplify_postfix(std::vector<std::string> &postfix)
    {
        std::stack<std::vector<std::string>> operands;
        for (auto &element : postfix)
        {
            if (func_map.find(element) != func_map.end() && element != "x")
            {
                auto func = func_map[element];
                if (func.two_var_flag)
                {
                    auto second = operands.top();
                    operands.pop();
                    auto first = operands.top();
                    operands.pop();
                    double first_num = -1;
                    double second_num = -1;
                    bool first_is_num = false;
                    bool second_is_num = false;
                    bool multiple_first = first.size() > 1;
                    bool multiple_second = second.size() > 1;
                    if (!multiple_first)
                    {
                        first_is_num = to_double(first[0], first_num);
                    }
                    if (!multiple_second)
                    {
                        second_is_num = to_double(second[0], second_num);
                    }
                    if (first_is_num && second_is_num)
                    {
                        auto result = func.func(first_num, second_num);
                        operands.push({double_to_string(result, 2)});
                    }
                    else if (element == "+") // кто-то не число
                    {
                        if (first_is_num && std::abs(first_num) < zero_limit)
                        {
                            operands.push(second);
                        }
                        else if (second_is_num && std::abs(second_num) < zero_limit)
                        {
                            operands.push(first);
                        }
                        else
                        {
                            first.insert(first.end(), second.begin(), second.end());
                            first.push_back("+");
                            operands.push(first);
                        }
                    }
                    else if (element == "-")
                    {
                        if (first_is_num && std::abs(first_num) < zero_limit)
                        {
                            for (int i = 0; i != second.size(); i++)
                            {
                                if (func_map.find(second[i]) != func_map.end())
                                    continue;
                                if (i % 2 != 1 || second[i + 1] != "*")
                                {
                                    if (second[i][0] == '-')
                                    {
                                        second[i].erase(second[i].begin());
                                    }
                                    else
                                    {
                                        second[i].insert(second[i].begin(), '-');
                                    }
                                }
                            }
                            operands.push(second);
                        }
                        else if (second_is_num && std::abs(second_num) < zero_limit)
                        {
                            operands.push(first);
                        }
                        else
                        {
                            first.insert(first.end(), second.begin(), second.end());
                            first.push_back("-");
                            operands.push(first);
                        }
                    }
                    else if (element == "*")
                    {
                        if ((first_is_num && std::abs(first_num) < zero_limit) ||
                            (second_is_num && std::abs(second_num) < zero_limit))
                        {
                            operands.push({"0"});
                        }
                        else if (first_is_num && std::abs(first_num - 1.0) < zero_limit)
                        {
                            operands.push(second);
                        }
                        else if (second_is_num && std::abs(second_num - 1.0) < zero_limit)
                        {
                            operands.push(first);
                        }
                        else
                        {
                            first.insert(first.end(), second.begin(), second.end());
                            first.push_back("*");
                            operands.push(first);
                        }
                    }
                    else if (element == "/")
                    {
                        if (first_is_num && std::abs(first_num) < zero_limit)
                        {
                            operands.push({"0"});
                        }
                        else if (second_is_num && std::abs(second_num - 1.0) < zero_limit)
                        {
                            operands.push(first);
                        }
                        else if (second_is_num && std::abs(second_num) < zero_limit)
                        {
                            throw std::overflow_error("Divide by zero exception");
                        }
                        else
                        {
                            first.insert(first.end(), second.begin(), second.end());
                            first.push_back("/");
                            operands.push(first);
                        }
                    }
                    else if (element == "^")
                    {
                        if ((first_is_num && std::abs(first_num - 1.0) < zero_limit) ||
                            (second_is_num && std::abs(second_num) < zero_limit))
                        {
                            operands.push({"1"});
                        }
                        else if (first_is_num && std::abs(first_num) < zero_limit)
                        {
                            operands.push({"0"});
                        }
                        else if (second_is_num && std::abs(second_num - 1.0) < zero_limit)
                        {
                            operands.push(first);
                        }
                        else
                        {
                            first.insert(first.end(), second.begin(), second.end());
                            first.push_back("^");
                            operands.push(first);
                        }
                    }
                }
                else
                {
                    auto operand = operands.top();
                    operands.pop();
                    double operand_num;
                    bool operand_is_num;
                    bool multiple_operand = operand.size() > 1;
                    if (!multiple_operand)
                    {
                        operand_is_num = to_double(operand[0], operand_num);
                    }
                    if (operand_is_num)
                    {
                        auto result = func.func(operand_num, 0);
                        operands.push({double_to_string(result, 2)});
                    }
                    else
                    {
                        operand.push_back(func.name);
                        operands.push(operand);
                    }
                }
            }
            else
            {
                operands.push({element});
            }
        }
        std::vector<std::string> result = operands.top();
        operands.pop();
        if (!operands.empty())
        {
            throw std::logic_error("Expression is invalid!");
        }
        // while (!operands.empty())
        // {
        //     std::swap(operands.top(), result);
        //     result.insert(result.end(), operands.top().begin(), operands.top().end());
        //     result.push_back("*");
        //     operands.pop();
        // }
        return result;
    }
    friend class syntaxTree;
    friend class syntaxNode;
};
#endif