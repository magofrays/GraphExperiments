#include "decoder.h"
#include "syntax_tree.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string view;
    std::getline(std::cin, view);
    syntaxTree tree(view);
    std::cout << tree.infix_tree_string() << "\n";
    tree.simplify_tree();
    std::cout << tree.infix_tree_string() << "\n";
    return 0;
}
