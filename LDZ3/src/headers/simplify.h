#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include "syntax_node.h"
syntaxNode *simplify(syntaxNode *node);

syntaxNode *simplify_both(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(node->type, node->func_info, node->value);
    new_node->left = simplify(node->left);
    new_node->right = simplify(node->right);
    if (new_node->left->type == syntaxNode::operand || new_node->right->type == syntaxNode::operand) // если больше не функции
    {
        auto result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return new_node;
}

syntaxNode *simplify_right(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(node->type, node->func_info, node->value);
    new_node->left = new syntaxNode(*(node->left));
    new_node->right = simplify(node->right);
    if (new_node->right->type == syntaxNode::operand)
    {
        auto result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return new_node;
}
syntaxNode *simplify_left(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(node->type, node->func_info, node->value);
    new_node->right = new syntaxNode(*(node->right));
    new_node->left = simplify(node->left);
    if (new_node->left->type == syntaxNode::operand)
    {
        auto result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return new_node;
}

syntaxNode *simplify_mul(syntaxNode *node)
{
    if (node->left->type == syntaxNode::operand) // left is operand
    {
        if (node->left->value == 0)
        {
            return new syntaxNode(syntaxNode::operand, nullptr, 0);
        }
        else if (node->left->value == 1)
        {
            return new syntaxNode(*(node->right));
        }
        return simplify_right(node);
    }
    else if (node->right->type == syntaxNode::operand) // right is operand
    {
        if (node->right->value == 0)
        {
            return new syntaxNode(syntaxNode::operand, nullptr, 0);
        }
        else if (node->right->value == 1)
        {
            return new syntaxNode(*(node->left));
        }
        return simplify_left(node);
    }
    return simplify_both(node);
}

syntaxNode *simplify_add(syntaxNode *node)
{
    if (node->left->type == syntaxNode::operand) // left is operand
    {
        if (node->left->value == 0)
        {
            return new syntaxNode(*(node->right));
        }
        return simplify_right(node);
    }
    else if (node->right->type == syntaxNode::operand) // right is operand
    {
        if (node->right->value == 0)
        {
            return new syntaxNode(*(node->left));
        }
        return simplify_left(node);
    }
    return simplify_both(node);
}

syntaxNode *simplify_sub(syntaxNode *node)
{
    if (node->left->type == syntaxNode::operand) // left is operand
    {
        if (node->left->value == 0)
        {
            syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["--"]));
            new_node->left = new syntaxNode(*(node->right));
            auto result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        return simplify_left(node);
    }
    else if (node->right->type == syntaxNode::operand) // right is operand
    {
        if (node->right->value == 0)
        {
            return new syntaxNode(*(node->left));
        }
        return simplify_right(node);
    }
    return simplify_both(node);
}

syntaxNode *simplify_minus(syntaxNode *node)
{
    if (node->left->func_info->name == "--")
    {
        return new syntaxNode(*(node->left->left));
    }
    return new syntaxNode(*(node));
}

syntaxNode *simplify_pow(syntaxNode *node)
{
    if (node->left->type == syntaxNode::operand) // left is operand
    {
        if (node->left->value == 0)
        {
            return new syntaxNode(syntaxNode::operand, nullptr, 1);
        }
        else if (node->left->value == 1)
        {
            return new syntaxNode(syntaxNode::operand, nullptr, 1);
        }
        return simplify_right(node);
    }
    else if (node->right->type == syntaxNode::operand) // right is operand
    {
        if (node->right->value == 0)
        {
            return new syntaxNode(syntaxNode::operand, nullptr, 0);
        }
        else if (node->right->value == 1)
        {
            return new syntaxNode(*(node->left));
        }
        return simplify_left(node);
    }
    return simplify_both(node);
}

syntaxNode *simplify_div(syntaxNode *node)
{
    if (node->left->type == syntaxNode::operand) // left is operand
    {
        if (node->left->value == 0)
        {
            return new syntaxNode(syntaxNode::operand, nullptr, 0);
        }
        return simplify_right(node);
    }
    else if (node->right->type == syntaxNode::operand) // right is operand
    {
        if (node->right->value == 0)
        {
            throw std::logic_error("division by zero!");
        }
        else if (node->right->value == 1)
        {
            return new syntaxNode(*(node->left));
        }
        return simplify_left(node);
    }
    if (postfix_node_string(node->left) == postfix_node_string(node->right))
    {
        return new syntaxNode(syntaxNode::operand, nullptr, 1);
    }
    return simplify_both(node);
}

syntaxNode *simplify(syntaxNode *node)
{
    if (node->type == syntaxNode::func && node->func_info->name != "x")
    {
        if (node->func_info->two_var_flag)
        {
            if (node->left->type == syntaxNode::operand && node->right->type == syntaxNode::operand)
            {
                double value = node->func_info->func(node->left->value, node->right->value);
                return new syntaxNode(syntaxNode::operand, nullptr, value);
            }
            else if (node->func_info->name == "*") // mul
            {
                return simplify_mul(node);
            }
            else if (node->func_info->name == "+") // add
            {
                return simplify_add(node);
            }
            else if (node->func_info->name == "-")
            {
                return simplify_sub(node);
            }
            else if (node->func_info->name == "^")
            {
                return simplify_pow(node);
            }
            else if (node->func_info->name == "/")
            {
                return simplify_div(node);
            }
        }
        else
        {
            if (node->left->type == syntaxNode::operand)
            {
                double value = node->func_info->func(node->left->value, 0);
                return new syntaxNode(syntaxNode::operand, nullptr, value);
            }
            else if (node->func_info->name == "--")
            {
                return simplify_minus(node);
            }
        }
    }
    return new syntaxNode(*node);
}

#endif