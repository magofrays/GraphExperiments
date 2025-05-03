#include "syntax_tree.h"
#include "derivative.h"
#include "simplify.h"

syntaxTree::syntaxTree(std::string &raw_text)
{
    auto first = Decoder::get()->parse(raw_text);
    auto second = Decoder::get()->postfix(first);
    auto third = Decoder::get()->simplify_postfix(second);
    create_tree(third);
}

void syntaxTree::create_tree(std::vector<std::string> &postfix)
{
    std::stack<syntaxNode *> operands;
    for (auto &element : postfix)
    {
        if (Decoder::get()->func_map.find(element) != Decoder::get()->func_map.end())
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
                auto *new_node = simplify(node);
                delete_node(node);
                operands.push(new_node);
            }
            else
            {
                auto *operand = operands.top();
                operands.pop();
                syntaxNode *node = new syntaxNode(syntaxNode::func, d_func, -1);
                node->left = operand;
                auto *new_node = simplify(node);
                delete_node(node);
                operands.push(new_node);
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

void syntaxTree::simplify_tree()
{
    auto result = simplify(root);
    delete_node(root);
    root = result;
}
void syntaxTree::find_derivative()
{
    syntaxNode *new_node = diff(root);
    delete_node(root);
    auto *new_root = simplify(new_node);
    delete_node(new_node);
    root = new_root;
}