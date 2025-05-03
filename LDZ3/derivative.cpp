#include "derivative.h"

syntaxNode *diff_mul(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["+"]));
    syntaxNode *new_left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
    syntaxNode *new_right = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
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
    syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["/"]));
    new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["-"]));
    new_node->right = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["^"]));
    new_node->right->right = new syntaxNode(syntaxNode::operand, nullptr, 2);
    new_node->right->left = new syntaxNode(*(node->right));

    new_node->left->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
    new_node->left->right = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));

    new_node->left->left->left = diff(node->left);
    new_node->left->left->right = new syntaxNode(*(node->right));
    new_node->left->right->left = new syntaxNode(*(node->left));
    new_node->left->right->right = diff(node->right);
    return new_node;
}

syntaxNode *diff_exp(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
    new_node->left = diff(node->left);
    new_node->right = new syntaxNode(*node);
    return new_node;
}

syntaxNode *diff_pow(syntaxNode *node)
{
    if ((node->right->type == syntaxNode::func) && (node->left->type == syntaxNode::func)) // случай функция в степени функции
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["exp"]));
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left->right = new syntaxNode(*(node->right));
        new_node->left->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["ln"]));
        new_node->left->left->left = new syntaxNode(*(node->left));
        auto result = diff(new_node);
        delete_node(new_node);
        return result;
    }
    else if (node->right->type == syntaxNode::func && node->right->info() == "x") // случай число в степени перменной
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->right = new syntaxNode(*node);
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["ln"]));
        new_node->left->left = new syntaxNode(*(node->left));
        return new_node;
    }
    else if (node->right->type == syntaxNode::func) // случай число в степени функции
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->right = new syntaxNode(*node);
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["ln"]));
        new_node->left->left->left = new syntaxNode(*(node->left));
        new_node->left->right = diff(node->right);
        return new_node;
    }
    else if (node->left->type == syntaxNode::func) // случай функция в степени числа
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->right = new syntaxNode(*node);
        double power = node->right->value;
        new_node->right->right->value = power - 1.0;
        new_node->left->left = new syntaxNode(*(node->right));
        new_node->left->right = diff(node->left);
        return new_node;
    }
}

syntaxNode *diff_ln(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["/"]));
    new_node->left = diff(node->left);
    new_node->right = new syntaxNode(*(node->left));
    return new_node;
}

syntaxNode *diff_sin(syntaxNode *node)
{
    if (node->left->info() != "x")
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->right = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["cos"]));
        new_node->left = diff(node->left);
        return new_node;
    }
    else
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["cos"]));
        new_node->left = new syntaxNode(*(node->left));
        return new_node;
    }
}
syntaxNode *diff_cos(syntaxNode *node)
{
    if (node->left->info() != "x")
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["*"]));
        new_node->right = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["~"]));
        new_node->right->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["sin"]));
        new_node->right->left->left = new syntaxNode(*(node->left));
        new_node->left = diff(node->left);
        return new_node;
    }
    else
    {
        syntaxNode *new_node = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["~"]));
        new_node->left = new syntaxNode(syntaxNode::func, &(Decoder::get()->func_map["sin"]));
        new_node->left->left = new syntaxNode(*(node->left));
        return new_node;
    }
}
syntaxNode *diff_minus(syntaxNode *node)
{
    syntaxNode *new_node = new syntaxNode(*node);
    new_node->left = diff(node->left);
    return new_node;
}

syntaxNode *diff(syntaxNode *node)
{
    if (node->type == syntaxNode::func)
    {
        if (node->info() == "+" || node->info() == "-" && node->right)
        {
            syntaxNode *new_node = new syntaxNode(*node);
            new_node->left = diff(node->left);
            new_node->right = diff(node->right);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "x")
        {
            syntaxNode *new_node = new syntaxNode(syntaxNode::operand, nullptr, 1);
            return new_node;
        }
        else if (node->info() == "*")
        {
            auto *new_node = diff_mul(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "/")
        {
            auto *new_node = diff_div(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "^")
        {
            auto *new_node = diff_pow(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "exp")
        {
            auto *new_node = diff_exp(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "ln")
        {
            auto *new_node = diff_ln(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "sin")
        {
            auto *new_node = diff_sin(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "cos")
        {
            auto *new_node = diff_cos(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
        else if (node->info() == "~")
        {
            auto *new_node = diff_minus(node);
            auto *result = simplify(new_node);
            delete_node(new_node);
            return result;
        }
    }

    syntaxNode *new_node = new syntaxNode(syntaxNode::operand, nullptr, 0);
    return new_node;
}
