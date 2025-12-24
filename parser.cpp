#include "parser.hpp"

#include "builtins.hpp"
#include "helpers.hpp"
#include "lexer.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

// this is a pratt parser
// some resources i have used to learn and implement this parser
// {
// https://www.youtube.com/watch?v=WdlXBDHXqAs
// https://www.youtube.com/watch?v=jIxsH3E-Hjg
// https://www.youtube.com/watch?v=2l1Si4gSb9A
// https://youtu.be/hi2D1K4tFT8?si=ZeVO4Bp9QD6OGpYU
// https://youtu.be/0c8b7YfsBKs?si=HCgZzwirEiO5ITql
// https://youtu.be/tM_S-pa4xDk?si=oyyK05sOt5blKeRl
// https://matklad.github.io/2020/04/13/simple-but-powerful-pratt-parsing.html#Pratt-parsing-the-general-shape
// https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/
// }

std::unordered_map<TokenTypes, std::tuple<int, int>> infixBindingPower = {
    {POWOP, std::make_tuple(60, 59)},  // the exact numbers dosen't matter the ratios do
    {MULOP, std::make_tuple(50, 51)},
    {DIVOP, std::make_tuple(50, 51)},
    {PLUSOP, std::make_tuple(40, 41)},
    {SUBOP, std::make_tuple(40, 41)}};

std::unordered_map<TokenTypes, int> prefixBidingPower = {{PLUSOP, 70}, {SUBOP, 70}};

std::unordered_map<TokenTypes, std::string> ttts = {{PLUSOP, "PLUSOP"},
                                                    {SUBOP, "SUBOP"},
                                                    {DIVOP, "DIVOP"},
                                                    {MULOP, "MULOP"},
                                                    {POWOP, "POWOP"},
                                                    {NUM, "NUM"},
                                                    {VAR, "VAR"},
                                                    {ENDOFFILE, "ENDOFFILE"},
                                                    {OPENPAR, "OPENPAR"},
                                                    {CLOSEDPAR, "CLOSEDPAR"},
                                                    {IDENTIFIER, "IDENTIFIER"}};

std::string TokenTypeToString(TokenTypes type) {
    return ttts[type];
}

tokenStream::tokenStream(std::vector<Token> tokens) {
    this->tokens = tokens;
    this->position = 0;
}

Token tokenStream::current() {
    if (static_cast<unsigned int>(this->position) < this->tokens.size()) {
        return this->tokens[this->position];
    }
}

void tokenStream::advance() {
    this->position++;
}

bool tokenStream::expect(TokenTypes expected, const Token& t) {
    bool isExpected = (current().type == expected);
    if (!isExpected)
        if (expected == CLOSEDPAR) {
            throw std::runtime_error(std::string {"forgot ) "} + " try adding ) at " +
                                     std::to_string(t.line) + ":" + std::to_string(t.col));
        }
    return isExpected;
}

int getPrefixBidingPower(TokenTypes tokenType) {
    if (prefixBidingPower.find(tokenType) != prefixBidingPower.end()) {
        return prefixBidingPower[tokenType];
    }

    throw std::runtime_error(TokenTypeToString(tokenType) +
                             " does not have a prefix binding power");
}

std::tuple<int, int> getInfixBindingPower(TokenTypes tokenType) {
    if (infixBindingPower.find(tokenType) != infixBindingPower.end()) {
        return infixBindingPower[tokenType];
    }

    return std::make_tuple(-1, -1);
}

double Parser::parse(std::vector<Token> tokens) {
    tokenStream ts = tokenStream(tokens);
    Expr result = parseExpression(ts, 0);
    if (ts.current().type != ENDOFFILE) {
        throw std::runtime_error("Unexpected token after expression: " +
                                 TokenTypeToString(ts.current().type));
    }
    return Parser::evaluate(result);
}

template <typename U, typename T>
bool Parser::isThenGet(T& variant, U& output) {
    if (std::holds_alternative<U>(variant)) {
        output = std::get<U>(variant);
        return true;
    }
    return false;
}

double Parser::evaluate(Expr AST) {
    // BinaryOp(PLUSOP):
    //   Number(2)
    //   BinaryOp(MULOP):
    //     Number(2)
    //     Number(4)
    //
    if (std::holds_alternative<std::shared_ptr<binaryOpNode>>(AST)) {
        binaryOpNode NODE = (*std::get<std::shared_ptr<binaryOpNode>>(AST));
        switch (NODE.op) {
        case PLUSOP:
            return Parser::evaluate(NODE.left) + Parser::evaluate(NODE.right);

        case SUBOP:

            return Parser::evaluate(NODE.left) - Parser::evaluate(NODE.right);

        case MULOP:

            return Parser::evaluate(NODE.left) * Parser::evaluate(NODE.right);
        case DIVOP:

            return Parser::evaluate(NODE.left) / Parser::evaluate(NODE.right);

        case POWOP:

            return std::pow(Parser::evaluate(NODE.left), Parser::evaluate(NODE.right));

        default:

            throw std::runtime_error("NOT IMPLEMENTED SWITCH");
        }
    } else if (std::holds_alternative<numberNode>(AST)) {
        return std::get<numberNode>(AST).value;
    } else if (std::holds_alternative<identifierNode>(AST)) {
        identifierNode n = std::get<identifierNode>(AST);
        std::string funcName = n.name;
        std::transform(funcName.begin(), funcName.end(), funcName.begin(), ::toupper);

        if (funcName == "SIN") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return std::sin((val * M_PI / 180.0));
            }
        } else if (funcName == "PI") {
            return M_PI;

        } else if (funcName == "INF") {
            return INFINITY;
        } else if (funcName == "ABS") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return std::abs(val);
            }
        } else if (funcName == "FLOOR") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return std::floor(val);
            }
        } else if (funcName == "CEIL") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return std::ceil(val);
            }
        } else if (funcName == "ROUND") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return std::round(val);
            }
        } else if (funcName == "TRUNC") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return std::trunc(val);
            }
        } else if (funcName == "FACTORIAL") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return FACTORIAL(val);
            }
        } else if (funcName == "RAND") {
            if (n.args.size() > 0) {
                double val = evaluate(n.args[0]);
                return RAND(val);
            }
            throw std::runtime_error("NOT IMPLEMENTED IFS");
        }
    }
}

Expr Parser::parseExpression(tokenStream& ts, int minBindingPower) {
    Token token = ts.current();
    ts.advance();

    // parse the PREFIX expression (the left side)
    //
    Expr left = parsePrefixExpression(token, ts);

    // the main parsing loop - this is where we handle INFIX operators
    // we keep looping as long as we see infix operators with high enough binding
    // power
    while (true) {
        token = ts.current();  // get current token for checking

        if (token.type == CLOSEDPAR || token.type == ENDOFFILE)
            // hit end of expression
            break;

        std::tuple<int, int> bindingPowers = getInfixBindingPower(token.type);
        int lbp = std::get<0>(bindingPowers);
        int rbp = std::get<1>(bindingPowers);

        if (lbp < 0)
            // not an infix operator
            break;

        // previous operator has more binding power than current operator
        // for example: 2*3+4 when we are parsing 3 the previous operator
        // would be * which has more binding power meaning we will leave 3
        // for it making it (2*3)+4
        if (lbp < minBindingPower)
            break;

        // if we get here this means that this operator binds to our left side
        // advance PAST the operator token
        ts.advance();

        Expr right = parseExpression(ts, rbp);
        left = std::make_shared<binaryOpNode>(
            binaryOpNode {token.type, std::move(left), std::move(right)});
    }

    return left;
}

Expr Parser::parsePrefixExpression(const Token& token, tokenStream& ts) {
    switch (token.type) {
    case NUM: {
        if (std::holds_alternative<double>(token.value))
            return numberNode {std::get<double>(token.value)};
        else
            throw std::runtime_error("TOKENTYPE VALUE MISMATCH");
    }

    case VAR: {
        if (std::holds_alternative<std::string>(token.value))
            return variableNode {std::get<std::string>(token.value)};
        else
            throw std::runtime_error("TOKENTYPE VALUE MISMATCH");
    }

    case IDENTIFIER: {
        Expr expr = parseExpression(ts, 0);
        return identifierNode {std::get<std::string>(token.value), {expr}};
    }
    case OPENPAR: {
        // parenthesis expression (....)
        // recursively parse its insides with binding power of 0
        // binding power of 0 means accept anything
        Expr expr = parseExpression(ts, 0);
        ts.expect(CLOSEDPAR,
                  token);  // we MUST have ) after parsing what's inside ()
        ts.advance();  // move past the CLOSEDPAR
        return expr;
    }

    case PLUSOP:  // this will fall through to SUBOP
    case SUBOP: {
        int power = getPrefixBidingPower(token.type);
        // recursively parse the operand with this binding power
        // this ensures -5*3 parses as (-5)*3 not -(5*3)
        Expr operand = parseExpression(ts, power);

        if (std::holds_alternative<numberNode>(operand)) {
            numberNode oprn = std::get<numberNode>(operand);
            return UnaryOpNode {token.type, oprn};
        } else if (std::holds_alternative<variableNode>(operand)) {
            variableNode oprn = std::get<variableNode>(operand);
            return UnaryOpNode {token.type, oprn};
        } else {
            throw std::runtime_error("Unary operator can only be applied to numbers or variables");
        }
    }

    default: {
        throw std::runtime_error("Unexpected token at start of expression: " +
                                 TokenTypeToString(token.type));
    }
    }
}
