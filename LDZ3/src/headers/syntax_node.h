#ifndef SYNTAX_NODE_H
#define SYNTAX_NODE_H
#include "decoder.h"

struct syntaxNode
{
    enum Type
    {
        func,
        operand
    };
    Type type;
    double value;
    func_container *func_info = nullptr;

    syntaxNode *left = nullptr;
    syntaxNode *right = nullptr;
    std::string info()
    {
        if (type == func)
            return func_info->name;
        else
            return double_to_string(value, 2);
    }
    syntaxNode(Type type, func_container *func_info = nullptr, double value = -1) : type(type)
    {
        if (type == func)
            this->func_info = func_info;
        else
            this->value = value;
    }
    syntaxNode(const syntaxNode &node) : type(node.type), value(node.value), func_info(node.func_info)
    {
        if (node.left)
            this->left = new syntaxNode(*(node.left));
        if (node.right)
            this->right = new syntaxNode(*(node.right));
    }

    double operator()(double x) const;
};

std::vector<std::string> infix_node(syntaxNode *node);
std::string infix_node_string(syntaxNode *node);
std::vector<std::string> postfix_node(syntaxNode *node);
std::string postfix_node_string(syntaxNode *node);
void delete_node(syntaxNode *node);

#endif