#ifndef INTERNAL_H
#define INTERNAL_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum TokenTypes {
    PLUSOP,  // 0
    SUBOP,  // 1
    DIVOP,  // 2
    MULOP,  // 3
    POWOP,  // 4
    NUM,  // 5
    VAR,  // 6
    NUL,  // 7
    OPENPAR,  // 8
    CLOSEDPAR,  // 9
                //  null but i can't use null cause fricking c++ reserved it
};

struct Token {
    std::variant<double, std::string> value {};
    TokenTypes type;
};

std::vector<Token> Lex(std::string equation, bool dump, bool verbose);

inline std::ostream& operator<<(std::ostream& o, const Token& token) {
    if (std::holds_alternative<double>(token.value))
        return o << "\nToken:\t" << std::get<double>(token.value) << "\nType:\t" << token.type;
    else
        return o << "\nToken:\t" << std::get<std::string>(token.value) << "\nType:\t" << token.type;
}

inline void printContainer(const std::vector<std::tuple<Token, std::string>>& vec) {
    std::cout << "[\n";
    for (const auto& [token, str] : vec) {
        std::cout << "  (" << token << ", \"" << str << "\")\n";
    }
    std::cout << "]\n";
}

inline void printContainer(const std::tuple<std::string, std::string, bool>& t) {
    const auto& [str1, str2, flag] = t;
    std::cout << "(" << str1 << ", " << str2 << ", " << std::boolalpha << flag << ")\n";
}

template <typename T>
void printContainer(const std::vector<T>& vec) {
    for (T item : vec) {
        std::cout << item << "\n";
    }
}

inline int writeToFile(std::string filename, const std::vector<Token>& tokens) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return 1;
    }

    for (const auto& token : tokens) {
        file << token << "\n";
    }

    file.close();
    return 0;
}
#endif
