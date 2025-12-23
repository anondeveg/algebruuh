#include "parser.h"

#include "internals.h"

#include <cmath>
#include <ios>
#include <istream>
#include <memory>
#include <stdexcept>
#include <type_traits>
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

std::unordered_map<TokenTypes, std::string> ttts = {
    {PLUSOP, "PLUSOP"},
    {SUBOP, "SUBOP"},
    {DIVOP, "DIVOP"},
    {MULOP, "MULOP"},
    {POWOP, "POWOP"},
    {NUM, "NUM"},
    {VAR, "VAR"},
    {NUL, "NUL"},
    {OPENPAR, "OPENPAR"},
    {CLOSEDPAR, "CLOSEDPAR"},
};

std::string TokenTypeToString(TokenTypes type) {
    return ttts[type];
}

tokenStream::tokenStream(std::vector<Token> tokens) {
    this->tokens = tokens;
    this->position = 0;
}

Token tokenStream::current() {
    if (this->position < this->tokens.size()) {
        return this->tokens[this->position];
    }
    return Token {"", NUL};
}

void tokenStream::advance() {
    this->position++;
}

bool tokenStream::expect(TokenTypes expected) {
    bool isExpected = (current().type == expected);
    if (!isExpected)
        throw("UNEXPECTED TYPE wanted " + TokenTypeToString(expected) + " GOT " +
              TokenTypeToString(current().type));
    return isExpected;
}

int getPrefixBidingPower(TokenTypes tokenType) {
    if (prefixBidingPower.find(tokenType) != prefixBidingPower.end()) {
        return prefixBidingPower[tokenType];
    }

    throw TokenTypeToString(tokenType) + " does not have a prefix binding power";
}

std::tuple<int, int> getInfixBindingPower(TokenTypes tokenType) {
    if (infixBindingPower.find(tokenType) != infixBindingPower.end()) {
        return infixBindingPower[tokenType];
    }

    return std::make_tuple(-1, -1);
}

Expr Parser::parse(std::vector<Token> tokens) {
    tokenStream ts = tokenStream(tokens);
    Expr result = parseExpression(ts, 0);

    if (ts.current().type != NUL) {
        throw "Unexpected token after expression: " + TokenTypeToString(ts.current().type);
    }
    std::cout << "\n" << "RESULT -> " << Parser::evaluate(result) << "\n";  // evaluate results
    return result;
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
        double accum {};
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

            throw std::runtime_error("NOT IMPLEMENTED");
        }
    } else if (std::holds_alternative<numberNode>(AST)) {
        return std::get<numberNode>(AST).value;
    } else {
        return std::get<numberNode>(AST).value;
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

        if (token.type == CLOSEDPAR || token.type == NUL)
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
            throw "TOKENTYPE VALUE MISMATCH";
    }

    case VAR: {
        if (std::holds_alternative<std::string>(token.value))
            return variableNode {std::get<std::string>(token.value)};
        else
            throw "TOKENTYPE VALUE MISMATCH";
    }

    case OPENPAR: {
        // parenthesis expression (....)
        // recursively parse its insides with binding power of 0
        // binding power of 0 means accept anything
        Expr expr = parseExpression(ts, 0);
        ts.expect(CLOSEDPAR);  // we MUST have ) after parsing what's inside ()
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
            throw "Unary operator can only be applied to numbers or variables";
        }
    }

    default: {
        throw "Unexpected token at start of expression: " + TokenTypeToString(token.type);
    }
    }
}
