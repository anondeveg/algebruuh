#ifndef INTERNAL_H
#define INTERNAL_H

#include <iostream>
#include <map>
#include <vector>


enum TokenTypes
{
    ADDOP,  // 0
    SUBOP,  // 1
    DIVOP,  // 2
    MULOP,  // 3
    POWOP,  // 4
    NUM,    // 5
    VAR,    // 6
    NUL,    // 7
            //  null but i can't use null cause fricking c++ reserved it
};
// struct Term
// {
//     double coefficient;
//     std::string variable{};
//     int power = 1;
//     std::string operation;
//     bool lhs;  // whether left hand side or no, will help later in AST generation
// };
//
struct Token
{
    std::string token;
    TokenTypes type;
};

std::vector<Token> Lex(std::string equation);

inline std::ostream& operator<<(std::ostream& o, const Token& token)
{
    return o << "\nToken:\t" << token.token << "\nType:\t" << token.type;
}

inline void printContainer(const std::tuple<std::string, std::string, bool>& t)
{
    const auto& [str1, str2, flag] = t;
    std::cout << "(" << str1 << ", " << str2 << ", " << std::boolalpha << flag << ")\n";
}
template <typename T>
void printContainer(const std::vector<T>& vec)
{
    for (T item : vec)
    {
        std::cout << item << "\n";
    }
}

#endif
