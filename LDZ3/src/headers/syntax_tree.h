#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include "syntax_node.h"

class syntaxTree
{
    syntaxNode *root;

public:
    syntaxTree() = default;

    syntaxTree(std::string &raw_text);

    double operator()(double x)
    {
        return (*root)(x);
    }

    void create_tree(std::vector<std::string> &postfix);

    std::string infix_tree_string()
    {
        return infix_node_string(root);
    }

    void simplify_tree();

    void find_derivative();

    ~syntaxTree()
    {
        delete_node(root);
    }
};

#endif