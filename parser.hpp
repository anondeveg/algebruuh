#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"

#include <memory>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

struct binaryOpNode;
struct variableNode;
struct numberNode;
struct UnaryOpNode;

// type alias for expression variant
using Expr = std::variant<variableNode, numberNode, std::shared_ptr<binaryOpNode>, UnaryOpNode>;

struct variableNode {
    std::string name;
};

struct numberNode {
    double value;
};

struct UnaryOpNode {
    TokenTypes op;
    std::variant<variableNode, numberNode> operand;
};

struct binaryOpNode {
    TokenTypes op;
    Expr left;
    Expr right;
};

using anyNode = std::variant<numberNode, variableNode, UnaryOpNode, binaryOpNode>;

class tokenStream {
  public:
    std::vector<Token> tokens;
    int position;

    tokenStream(std::vector<Token> tokens);
    Token current();
    void advance();
    bool expect(TokenTypes expected);
};

// helper functions
std::string TokenTypeToString(TokenTypes type);
int getPrefixBidingPower(TokenTypes tokenType);
std::tuple<int, int> getInfixBindingPower(TokenTypes tokenType);

class Parser {
  public:
    static Expr parse(std::vector<Token> tokens);
    static Expr parseExpression(tokenStream& ts, int minBindingPower);
    static Expr parsePrefixExpression(const Token& token, tokenStream& ts);
    static double evaluate(Expr AST);

    template <typename U, typename T>
    static bool isThenGet(T& variant, U& output);
};

#endif
