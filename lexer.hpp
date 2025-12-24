#pragma once
#include <iostream>
#include <optional>
#include <string>
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
    NUL,  // 7 null but i can't use null cause c++ reserved it
    OPENPAR,  // 8
    CLOSEDPAR,  // 9
    IDENTIFIER,  // 10
    COMMA,  // 11
    UNKNOWN,  // 12
    ENDOFFILE,  // 13
    ASSIGN, // 14
		FUNCTION, // 15
		SEMICOLON, // 16
};

struct Token {
    TokenTypes type;
    std::variant<double, std::string> value {};
    int line;
    int col;
};


class Lexer {
	public:
    std::string source_code;
    int pos;  // position but zero indexed for actual indexing
    int line;  // line number 1 indexed for error messages
    int column;  // same as line
    std::optional<char> currentChar;

    // most lexers also have a keywords hashmap,but beacause this language is so simple i don't need
    // it (now at least)

    Lexer(std::string source_code);

    void advance();

    std::optional<char> peek(int offset);

    void skipWhitSpace();
    void skipComments();
    Token readNumber();
    Token readAlphanumeric();

    Token genNextToken();

    std::vector<Token> tokenize();
};

inline std::ostream& operator<<(std::ostream& o, const Token& token) {
    if (std::holds_alternative<double>(token.value))
        return o << "\nToken:\t" << std::get<double>(token.value) << "\nType:\t" << token.type;
    else
        return o << "\nToken:\t" << std::get<std::string>(token.value) << "\nType:\t" << token.type;
}

