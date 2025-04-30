#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include <string>
#include <functional>

struct Node
{
    std::string info;
    std::function<double(double)> m_func;
    Node *left;
    Node *right;

    Node(std::string info, std::function<double(double)> m_fund) : info(info), m_func(m_func)
    {
    }
    double call(double x) const
    {
        return m_func(x);
    }
};

class Tree
{
    Node *root;
    Tree();
};

#endif