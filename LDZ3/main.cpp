#include "decoder.h"
#include "syntax_tree.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string view;
    std::getline(std::cin, view);
    syntaxTree tree(view);
    // std::cout << tree.infix_tree_string() << "\n";
    tree.find_derivative();

    std::cout << tree.infix_tree_string() << "\n";
    // double x;
    // std::cin >> x;
    // std::cout << tree(x) << "\n";
    return 0;
}
