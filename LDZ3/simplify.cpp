#include "simplify.h"
#include "syntax_node.h"

syntaxNode *simplify_both(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(node->type, node->func_info, node->value);
    new_node->left = simplify(node->left);
    new_node->right = simplify(node->right);
    if (new_node->left->type == syntaxNode::operand || new_node->right->type == syntaxNode::operand) // если больше не функции
    {
        auto *result = simplify(new_node);
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
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return new_node;
}
syntaxNode *simplify_left(syntaxNode *node, bool one_var)
{
    syntaxNode *new_node = new syntaxNode(node->type, node->func_info, node->value);
    if (!one_var)
        new_node->right = new syntaxNode(*(node->right));
    new_node->left = simplify(node->left);
    if (new_node->left->type == syntaxNode::operand)
    {
        auto *result = simplify(new_node);
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
    if (node->left->func_info->name == "/" && node->right->func_info->name == "/")
    {
        if (postfix_node_string(node->left->right) == postfix_node_string(node->right->left))
        {
            auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["/"]));
            new_node->left = new syntaxNode(*(node->left->left));
            new_node->right = new syntaxNode(*(node->right->right));
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (postfix_node_string(node->left->left) == postfix_node_string(node->right->right))
        {
            auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["/"]));
            new_node->left = new syntaxNode(*(node->right->left));
            new_node->right = new syntaxNode(*(node->left->right));
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
    }
    else if (node->left->func_info->name == "/")
    {
        auto *new_node = new syntaxNode(node->left->type, node->left->func_info, node->left->value);
        new_node->right = new syntaxNode(*(node->left->right));
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left->left = new syntaxNode(*(node->right));
        new_node->left->right = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->right->func_info->name == "/")
    {
        auto *new_node = new syntaxNode(node->right->type, node->right->func_info, node->right->value);
        new_node->right = new syntaxNode(*(node->right->right));
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left->left = new syntaxNode(*(node->left));
        new_node->left->right = new syntaxNode(*(node->right->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
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
    if (postfix_node_string(node->left) == postfix_node_string(node->right))
    {
        auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left = new syntaxNode(syntaxNode::operand, nullptr, 2);
        new_node->right = new syntaxNode(*(node->right));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->right->func_info->name == "*" && node->right->left->type == syntaxNode::operand &&
             postfix_node_string(node->right->right) == postfix_node_string(node->left))
    {
        auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left = new syntaxNode(syntaxNode::operand, nullptr, node->right->left->value + 1);
        new_node->right = new syntaxNode(*(node->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->left->func_info->name == "*" && node->left->left->type == syntaxNode::operand &&
             postfix_node_string(node->left->right) == postfix_node_string(node->right))
    {
        auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left = new syntaxNode(syntaxNode::operand, nullptr, node->left->left->value + 1);
        new_node->right = new syntaxNode(*(node->right));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->left->func_info->name == "*" && node->right->func_info->name == "*" &&
             postfix_node_string(node->left->right) == postfix_node_string(node->right->right) &&
             node->left->left->type == syntaxNode::operand && node->right->left->type == syntaxNode::operand)
    {
        auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left = new syntaxNode(syntaxNode::operand, nullptr, node->left->left->value + node->right->left->value);
        new_node->right = new syntaxNode(*(node->left->right));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->right->func_info->name == "~" && postfix_node_string(node->left) == postfix_node_string(node->right->left) ||
             node->left->func_info->name == "~" && postfix_node_string(node->right) == postfix_node_string(node->left->left))
    {
        auto *new_node = new syntaxNode(syntaxNode::operand, nullptr, 0);
        return new_node;
    }
    return simplify_both(node);
}

syntaxNode *simplify_sub(syntaxNode *node)
{
    if (node->left->type == syntaxNode::operand) // left is operand
    {
        if (node->left->value == 0)
        {
            syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["~"]));
            new_node->left = new syntaxNode(*(node->right));
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
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
    if (postfix_node_string(node->left) == postfix_node_string(node->right))
    {
        return new syntaxNode(syntaxNode::operand, nullptr, 0);
    }
    return simplify_both(node);
}

syntaxNode *simplify_minus(syntaxNode *node)
{
    if (node->left->func_info->name == "~")
    {
        return new syntaxNode(*(node->left->left));
    }
    return simplify_left(node, true);
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

syntaxNode *simplify_sin(syntaxNode *node)
{
    if (node->left->func_info->name == "arcsin")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_cos(syntaxNode *node)
{
    if (node->left->func_info->name == "arccos")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_arcsin(syntaxNode *node)
{
    if (node->left->func_info->name == "sin")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}
syntaxNode *simplify_arccos(syntaxNode *node)
{
    if (node->left->func_info->name == "cos")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_tg(syntaxNode *node)
{
    if (node->left->func_info->name == "arctg")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_ctg(syntaxNode *node)
{
    if (node->left->func_info->name == "arcctg")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_arctg(syntaxNode *node)
{
    if (node->left->func_info->name == "tg")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_arcctg(syntaxNode *node)
{
    if (node->left->func_info->name == "ctg")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_sqrt(syntaxNode *node)
{
    auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["^"]));
    new_node->left = new syntaxNode(*(node->left));
    new_node->right = new syntaxNode(syntaxNode::operand, nullptr, 0.5);
    auto *result = simplify(new_node);
    delete new_node;
    return result;
}

syntaxNode *simplify_exp(syntaxNode *node)
{
    if (node->left->func_info->name == "ln")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->left->func_info->name == "*")
    {
        if (node->left->left->func_info->name == "ln")
        {
            auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["^"]));
            new_node->left = new syntaxNode(*(node->left->left->left));
            new_node->right = new syntaxNode(*(node->left->right));
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->left->right->func_info->name == "ln")
        {
            auto *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["^"]));
            new_node->left = new syntaxNode(*(node->left->left));
            new_node->right = new syntaxNode(*(node->left->right));
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
    }
    return simplify_left(node, true);
}

syntaxNode *simplify_ln(syntaxNode *node)
{
    if (node->left->func_info->name == "exp")
    {
        auto *new_node = new syntaxNode(*(node->left->left));
        auto *result = simplify(new_node);
        delete_node(new_node);
        return result;
    }
    return simplify_left(node, true);
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
            else if (node->func_info->name == "~")
            {
                return simplify_minus(node);
            }
            else if (node->func_info->name == "exp")
            {
                return simplify_exp(node);
            }
            else if (node->func_info->name == "ln")
            {
                return simplify_ln(node);
            }
            else if (node->func_info->name == "sqrt")
            {
                return simplify_sqrt(node);
            }
            else if (node->func_info->name == "sin")
            {
                return simplify_sin(node);
            }
            else if (node->func_info->name == "cos")
            {
                return simplify_cos(node);
            }
            else if (node->func_info->name == "tg")
            {
                return simplify_tg(node);
            }
            else if (node->func_info->name == "ctg")
            {
                return simplify_ctg(node);
            }
            else if (node->func_info->name == "arcsin")
            {
                return simplify_arcsin(node);
            }
            else if (node->func_info->name == "arccos")
            {
                return simplify_arccos(node);
            }
            else if (node->func_info->name == "arctg")
            {
                return simplify_arctg(node);
            }
            else if (node->func_info->name == "arcctg")
            {
                return simplify_arcctg(node);
            }
        }
    }
    return new syntaxNode(*node);
}