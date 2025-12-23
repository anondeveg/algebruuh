
#include "lexer.hpp"

#include "helpers.hpp"

#include <bits/stdc++.h>
#include <cctype>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

Lexer::Lexer(std::string source_code) {
    this->source_code = source_code;
    this->pos = 0;
    this->line = 1;
    this->column = 1;
    this->currentChar = this->source_code[0];
}

void Lexer::advance() {
    if (this->currentChar.has_value() && this->currentChar.value() == '\n') {
        this->line += 1;
        this->column = 0;
    }

    this->pos++;
		this->column++;
    if (static_cast<unsigned int>(this->pos) >= this->source_code.size()) {
        this->currentChar = std::nullopt;  // reached end of file;
    } else {
        this->currentChar = this->source_code[this->pos];
    }
}

std::optional<char> Lexer::peek(int offset = 1) {
    int newpos = this->pos + offset;
    if (static_cast<unsigned int>(newpos) > this->source_code.size()) {
        return {};
    }
    return this->source_code[newpos];
}

void Lexer::skipWhitSpace() {
    while (this->currentChar.has_value() &&
           std::isspace(static_cast<unsigned char>(this->currentChar.value())))

        this->advance();
}

void Lexer::skipComments() {
    if (this->currentChar.value() == '#') {
        while (this->currentChar.has_value() && this->currentChar.value() != '\n') {
            this->advance();
        }

        this->advance();  // skip the last \n;
    }
}

Token Lexer::readNumber() {
    int startLine = this->line;
    int startColumn = this->column;
    std::string numberBuffer;

    while (this->currentChar.has_value() && std::isdigit(this->currentChar.value())) {
        numberBuffer += this->currentChar.value();
        this->advance();
    }

    if (this->currentChar.has_value() && this->currentChar.value() == '.' &&
        this->peek().has_value() && std::isdigit(this->peek().value())) {
        numberBuffer += this->currentChar.value();
        this->advance();
    }
    // after decimal
    while (this->currentChar.has_value() && std::isdigit(this->currentChar.value())) {
        numberBuffer += this->currentChar.value();
        this->advance();
    }
    return Token {NUM, std::stod(numberBuffer), startLine, startColumn};
}

Token Lexer::readIdentifier() {
    int startLine = this->line;
    int startColumn = this->column;
    std::string identifierBuffer;
    // read alphanumeric and _
    while (this->currentChar.has_value() &&
           (std::isalnum(this->currentChar.value()) || this->currentChar.value() == '_')) {
        identifierBuffer += this->currentChar.value();
        this->advance();
    }
    return Token {IDENTIFIER, identifierBuffer, startLine, startColumn};
}

Token Lexer::genNextToken() {
    while (this->currentChar.has_value()) {
        if (std::isspace(this->currentChar.value())) {
            this->skipWhitSpace();
            continue;
        }
        if (this->currentChar.value() == '#') {
            this->skipComments();
            continue;
        }

        break;
    }
    int startLine = this->line;
    int startColumn = this->column;

    if (std::isdigit(this->currentChar.value())) {
        return this->readNumber();
    }
    if (std::isalnum(this->currentChar.value()) || this->currentChar.value() == '_') {
        return this->readIdentifier();
    }

    std::unordered_map<char, TokenTypes> singleCharachterTokens = {
        {'+', PLUSOP},
        {'-', SUBOP},
        {'*', MULOP},
        {'/', DIVOP},
        {'(', OPENPAR},
        {')', CLOSEDPAR},
        {',', COMMA},
        {'^', POWOP},
        {'=', ASSIGN},
    };
    if (singleCharachterTokens.contains(this->currentChar.value())) {
        char ch = this->currentChar.value();
        TokenTypes ttype = singleCharachterTokens[ch];
        this->advance();
        return Token(ttype, std::string {ch}, startLine, startColumn);
    }

    char unknownChar = this->currentChar.value();
    this->advance();
    return Token(UNKNOWN, std::string {unknownChar}, startLine, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (this->currentChar.has_value()) {
        Token token = this->genNextToken();
        tokens.emplace_back(token);
    }

		tokens.emplace_back(Token{ENDOFFILE,"",this->line,this->column});
    printContainer(tokens);
    return tokens;
}
