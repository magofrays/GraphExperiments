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
        {
            return func_info->name;
        }
        else
        {
            return double_to_string(value, 2);
        }
    }
    syntaxNode(Type type, func_container *func_info = nullptr, double value = -1) : type(type)
    {
        if (type == func)
        {
            this->func_info = func_info;
        }
        else
        {
            this->value = value;
        }
    }
    syntaxNode(const syntaxNode &node) : type(node.type), value(node.value), func_info(node.func_info)
    {
        if (node.left)
            this->left = new syntaxNode(*(node.left));
        if (node.right)
            this->right = new syntaxNode(*(node.right));
    }

    double operator()(double x) const
    {
        if (type == syntaxNode::func)
        {
            if (func_info->two_var_flag)
            {
                double left_res = (*left)(x);
                double right_res = (*right)(x);
                return func_info->func(left_res, right_res);
            }

            else if (func_info->name == "x")
            {
                return func_info->func(x, 0);
            }

            else
            {
                double left_res = (*left)(x);
                return func_info->func(left_res, 0);
            }
        }
        return value;
    }
};

std::vector<std::string> infix_node(syntaxNode *node)
{
    if (!node)
        return {};
    if (node->type == syntaxNode::func && node->info() != "x")
    {
        std::vector<std::string> result;
        if (node->func_info->two_var_flag)
        {
            result.push_back("(");
            auto left = infix_node(node->left);
            auto right = infix_node(node->right);
            result.insert(result.end(), left.begin(), left.end());
            result.push_back(node->info());
            result.insert(result.end(), right.begin(), right.end());
            result.push_back(")");
            return result;
        }
        else
        {
            result.push_back(node->info());
            result.push_back("(");
            auto left = infix_node(node->left);
            result.insert(result.end(), left.begin(), left.end());
            result.push_back(")");
            return result;
        }
    }
    return {node->info()};
}

std::string infix_node_string(syntaxNode *node)
{
    if (!node)
        return {};
    if (node->type == syntaxNode::func && node->info() != "x")
    {
        std::vector<std::string> result;
        if (node->func_info->two_var_flag)
        {
            return "(" + infix_node_string(node->left) + " " + node->info() + " " + infix_node_string(node->right) + ")";
        }
        else
        {
            return node->info() + "(" + infix_node_string(node->left) + ")";
        }
    }
    return {node->info()};
}

std::vector<std::string> postfix_node(syntaxNode *node)
{
    if (!node)
        return {};
    if (node->type == syntaxNode::func && node->info() != "x")
    {
        std::vector<std::string> result;
        if (node->func_info->two_var_flag)
        {
            auto left = postfix_node(node->left);
            auto right = postfix_node(node->right);
            result.insert(result.begin(), left.begin(), left.end());
            result.insert(result.begin(), right.begin(), right.end());
            result.push_back(node->info());
            return result;
        }
        else
        {
            auto left = postfix_node(node->left);
            result.insert(result.begin(), left.begin(), left.end());
            result.push_back(node->info());
            return result;
        }
    }
    return {node->info()};
}

std::string postfix_node_string(syntaxNode *node)
{
    if (!node)
        return "";
    if (node->type == syntaxNode::func && node->info() != "x")
    {
        std::vector<std::string> result;
        if (node->func_info->two_var_flag)
        {
            return postfix_node_string(node->left) + " " + postfix_node_string(node->right) + " " + node->info();
        }
        else
        {
            return postfix_node_string(node->left) + " " + node->info();
        }
    }
    return node->info();
}

void delete_node(syntaxNode *node)
{
    if (!node)
        return;
    if (!node->left)
        delete_node(node->left);
    if (!node->right)
        delete_node(node->right);
    delete node;
}
#endif