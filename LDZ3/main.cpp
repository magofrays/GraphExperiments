#include "decoder.h"
#include "syntax_tree.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string view;
    std::getline(std::cin, view);
    syntaxTree tree(view);

    tree.find_derivative();

    std::cout << tree.infix_tree_string() << "\n";
    return 0;
}
