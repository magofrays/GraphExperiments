#include "decoder.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string view;
    std::getline(std::cin, view);
    Decoder decode;
    auto result = decode.parse(view);

    auto postfix = decode.postfix(result);
    auto simplified = decode.simplify_postfix(postfix);

    for (auto t : simplified)
    {
        std::cout << t << " ";
    }
    std::cout << "\n";
    return 0;
}
