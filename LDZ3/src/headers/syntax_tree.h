#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include "syntax_node.h"
#include "simplify.h"
#include "derivative.h"

class syntaxTree
{
    syntaxNode *root;
    Decoder decode;

public:
    syntaxTree() = default;

    syntaxTree(std::string &raw_text)
    {
        auto first = decode.parse(raw_text);
        auto second = decode.postfix(first);
        create_tree(second);
        simplify_tree();
    }

    double operator()(double x)
    {
        return (*root)(x);
    }

    void create_tree(std::vector<std::string> &postfix)
    {
        std::stack<syntaxNode *> operands;
        for (auto &element : postfix)
        {
            if (decode.func_map.find(element) != decode.func_map.end())
            {
                func_container *d_func = &(Decoder::get()->func_map[element]);
                if (element == "x")
                {
                    syntaxNode *node = new syntaxNode(syntaxNode::func, d_func, -1);
                    operands.push(node);
                }
                else if (d_func->two_var_flag)
                {
                    auto *right = operands.top();
                    operands.pop();
                    auto *left = operands.top();
                    operands.pop();
                    syntaxNode *node = new syntaxNode(syntaxNode::func, d_func, -1);
                    node->left = left;
                    node->right = right;
                    operands.push(node);
                }
                else
                {
                    auto *operand = operands.top();
                    operands.pop();
                    syntaxNode *node = new syntaxNode(syntaxNode::func, d_func, -1);
                    node->left = operand;
                    operands.push(node);
                }
            }
            else
            {
                double value;
                to_double(element, value);
                syntaxNode *node = new syntaxNode(syntaxNode::operand, nullptr, value);
                operands.push(node);
            }
        }
        root = operands.top();
        operands.pop();
        if (!operands.empty())
        {
            throw std::logic_error("Expression is invalid!");
        }
    }

    std::vector<std::string> infix_tree()
    {
        return infix_node(root);
    }

    std::string infix_tree_string()
    {
        return infix_node_string(root);
    }

    // void simplify()
    // {
    //     std::vector<std::string> postfix;
    //     postfix_node(root, postfix);
    //     auto simplified = decode.simplify_postfix(postfix);
    //     delete_node(root);
    //     create_tree(simplified);
    // }
    void simplify_tree()
    {
        auto result = simplify(root);
        delete_node(root);
        root = result;
    }

    void find_derivative()
    {
        syntaxNode *new_node = diff(root);
        delete_node(root);
        auto *new_root = simplify(new_node);
        delete_node(new_node);
        root = new_root;
    }

    ~syntaxTree()
    {
        delete_node(root);
    }
};

#endif