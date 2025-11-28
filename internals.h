#ifndef INTERNAL_H
#define INTERNAL_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

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

struct Token
{
    std::string token;
    TokenTypes type;
};




std::vector<Token> Lex(std::string equation,bool dump,bool verbose);

inline std::ostream& operator<<(std::ostream& o, const Token& token)
{
    return o << "\nToken:\t" << token.token << "\nType:\t" << token.type;
}

inline void printContainer(const std::vector<std::tuple<Token, std::string>>& vec)
{
    std::cout << "[\n";
    for (const auto& [token, str] : vec)
    {
        std::cout << "  (" << token << ", \"" << str << "\")\n";
    }
    std::cout << "]\n";
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

inline int writeToFile(std::string filename, const std::vector<Token>& tokens)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return 1;
    }
    
    for (const auto& token : tokens)
    {
        file << token << "\n";
    }
    
    file.close();
    return 0;
}
#endif
