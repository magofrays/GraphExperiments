#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include <string>
#include <functional>
#include "decoder.h"

struct syntaxNode
{
    enum Type
    {
        func,
        operand,
        variable
    };
    Type type;
    std::string info;
    double value;
    Decoder::func *func_info = nullptr;

    syntaxNode *left = nullptr;
    syntaxNode *right = nullptr;
    syntaxNode *parent = nullptr;

    syntaxNode(std::string info, Type type) : info(info), type(type) {}
    syntaxNode(const syntaxNode &node) : type(node.type), info(node.info), value(node.value), func_info(node.func_info)
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
            double left_res = (*left)(x);
            double right_res = 0;
            if (func_info->two_var_flag)
            {
                right_res = (*right)(x);
            }
            return func_info->func(left_res, right_res);
        }
        else if (type == syntaxNode::operand)
        {
            return value;
        }
        else if (type == syntaxNode::variable)
        {
            return func_info->func(x, 0);
        }
    }
};

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
        auto third = decode.simplify_postfix(second);
        create_tree(third);
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
                Decoder::func *d_func = &decode.func_map[element];
                if (element == "x")
                {
                    syntaxNode *node = new syntaxNode("x", syntaxNode::variable);
                    node->func_info = d_func;
                    operands.push(node);
                }
                else if (d_func->two_var_flag)
                {
                    auto right = operands.top();
                    operands.pop();
                    auto left = operands.top();
                    operands.pop();
                    syntaxNode *node = new syntaxNode(element, syntaxNode::func);
                    node->func_info = d_func;
                    node->left = left;
                    node->right = right;
                    left->parent = node;
                    right->parent = node;
                    operands.push(node);
                }
                else
                {
                    auto operand = operands.top();
                    operands.pop();
                    syntaxNode *node = new syntaxNode(element, syntaxNode::func);
                    node->func_info = d_func;
                    node->left = operand;
                    operand->parent = node;
                    operands.push(node);
                }
            }
            else
            {
                syntaxNode *node = new syntaxNode(element, syntaxNode::operand);
                double value;
                decode.to_double(element, value);
                node->value = value;
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
    std::string infix_node(syntaxNode *node)
    {
        if (!node)
            return "";
        if (node->type == syntaxNode::func)
        { // Если это оператор
            if (node->func_info->two_var_flag)
                return "(" + infix_node(node->left) + " " + node->info + " " + infix_node(node->right) + ")";
            else
                return node->info + "(" + infix_node(node->left) + ")";
        }
        return node->info;
    }
    std::string infix_tree()
    {
        return infix_node(root);
    }

    std::string postfix_node(syntaxNode *node)
    {
        if (!node)
            return "";
        if (node->type == syntaxNode::func)
        {
            if (node->func_info->two_var_flag)
                return postfix_node(node->left) + " " + postfix_node(node->right) + " " + node->info + " ";
            else
                return postfix_node(node->left) + " " + node->info + " ";
        }
        return node->info;
    }
    void simplify()
    {
        auto postfix = postfix_node(root);
        auto parsed = decode.parse(postfix);
        auto simplified = decode.simplify_postfix(parsed);
        delete_node(root);
        create_tree(simplified);
    }
    syntaxNode *diff_mul(syntaxNode *node)
    {
        syntaxNode *new_node = new syntaxNode("+", syntaxNode::func);
        syntaxNode *new_left = new syntaxNode("*", syntaxNode::func);
        syntaxNode *new_right = new syntaxNode("*", syntaxNode::func);
        new_node->func_info = &decode.func_map["+"];
        new_left->func_info = &decode.func_map["*"];
        new_right->func_info = &decode.func_map["*"];
        new_node->left = new_left;
        new_node->right = new_right;
        new_left->left = diff(node->left);
        new_left->right = new syntaxNode(*(node->right));
        new_right->left = new syntaxNode(*(node->left));
        new_right->right = diff(node->right);
        return new_node;
    }
    syntaxNode *diff_div(syntaxNode *node)
    {
        syntaxNode *new_node = new syntaxNode("/", syntaxNode::func);
        new_node->left = new syntaxNode("-", syntaxNode::func);
        new_node->right = new syntaxNode("^", syntaxNode::func);
        new_node->func_info = &decode.func_map["+"];
        new_node->left->func_info = &decode.func_map["-"];
        new_node->right->func_info = &decode.func_map["^"];
        new_node->right->right = new syntaxNode("2", syntaxNode::operand);
        new_node->right->right->value = 2;
        new_node->right->left = new syntaxNode(*(node->right));

        new_node->left->left = new syntaxNode("*", syntaxNode::func);
        new_node->left->left->func_info = &decode.func_map["*"];
        new_node->left->right = new syntaxNode("*", syntaxNode::func);
        new_node->left->right->func_info = &decode.func_map["*"];

        new_node->left->left->left = diff(node->left);
        new_node->left->left->right = new syntaxNode(*(node->right));
        new_node->left->right->left = new syntaxNode(*(node->left));
        new_node->left->right->right = diff(node->right);
        return new_node;
    }

    syntaxNode *diff_exp(syntaxNode *node)
    {
        syntaxNode *new_node = new syntaxNode("*", syntaxNode::func);
        new_node->func_info = &decode.func_map["*"];
        new_node->left = diff(node->left);
        new_node->right = new syntaxNode(*node);
        return new_node;
    }

    syntaxNode *diff_pow(syntaxNode *node)
    {
        syntaxNode *new_node = new syntaxNode("exp", syntaxNode::func);
        new_node->func_info = &decode.func_map["exp"];
        new_node->left = new syntaxNode("*", syntaxNode::func);
        new_node->left->func_info = &decode.func_map["*"];
        new_node->left->left = new syntaxNode(*(node->right));
        new_node->left->right = new syntaxNode("ln", syntaxNode::func);
        new_node->left->right->func_info = &decode.func_map["ln"];
        new_node->left->right->left = new syntaxNode(*(node->left));
        return diff(new_node);
    }

    syntaxNode *diff_ln(syntaxNode *node)
    {
        syntaxNode *new_node = new syntaxNode("/", syntaxNode::func);
        new_node->func_info = &decode.func_map["/"];
        new_node->left = diff(node->left);
        new_node->right = new syntaxNode(*(node->left));
        return new_node;
    }
    syntaxNode *diff(syntaxNode *node)
    {
        if (node->type == syntaxNode::func)
        {
            if (node->info == "+" || node->info == "-")
            {
                node->left = diff(node->left);
                node->right = diff(node->right);
            }
            else if (node->info == "*")
            {
                return diff_mul(node);
            }
            else if (node->info == "/")
            {
                return diff_div(node);
            }
            else if (node->info == "^")
            {
                return diff_pow(node);
            }
            else if (node->info == "exp")
            {
                return diff_exp(node);
            }
            else if (node->info == "ln")
            {
                return diff_ln(node);
            }

            // else if (node->info == "sin")
            // {
            //     if (node->left->type == syntaxNode::func)
            //     {
            //         syntaxNode *new_node = new syntaxNode("*", syntaxNode::func);
            //         syntaxNode *new_left;
            //     }
            // }
        }
        else if (node->type == syntaxNode::variable)
        {
            syntaxNode *new_node = new syntaxNode("1", syntaxNode::operand);
            new_node->value = 1;
            return new_node;
        }
        else
        {
            syntaxNode *new_node = new syntaxNode("0", syntaxNode::operand);
            new_node->value = 0;
            return new_node;
        }
    }

    void find_derivative()
    {
        root = diff(root);
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
    ~syntaxTree()
    {
        delete_node(root);
    }
};

#endif