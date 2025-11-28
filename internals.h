#ifndef INTERNAL_H
#define INTERNAL_H

#include <iostream>
#include <map>
#include <vector>

using termMap = std::map<int, std::tuple<std::string, std::string, bool>>;

enum TokenTypes
{
    ADDOP,
    SUBOP,
    DIVOP,
    MULOP,
    POWOP,
    NUM,
    VAR
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
struct Token{
	std::string token;
	TokenTypes type;


};

std::vector<Token> Lex(std::string equation);

inline std::ostream& operator<<(std::ostream& o, const Token& token )
{
    return o << "\nToken:\t" << token.token << "\nType:\t" << token.type;
}

inline void printContainer(const termMap& m, std::string_view comment = "")
{
    std::cout << comment;
    // Iterate using C++17 facilities
    for (const auto& [key, value] : m)
    {
        const auto& [str1, str2, flag] = value;
        std::cout << '[' << key << "] = (" << str1 << ", " << str2 << ", " << std::boolalpha << flag
                  << "); ";
    }
    std::cout << '\n';
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
