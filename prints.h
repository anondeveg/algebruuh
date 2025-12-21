#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "parser.h"

#include <iostream>
#include <memory>
#include <string>
#include <variant>

// THIS HEADER FILE WAS CREATED BY AI (don't have time for it) will rewrite
// later.

// Helper function to create indentation string
inline std::string getIndent(int indent) {
    return std::string(indent * 2, ' ');
}

// Forward declaration for recursive printing
void printExpr(const Expr& expr, int indent = 0);

// Print a numberNode
inline void printNode(const numberNode& node, int indent) {
    std::cout << getIndent(indent) << "Number(" << node.value << ")\n";
}

// Print a variableNode
inline void printNode(const variableNode& node, int indent) {
    std::cout << getIndent(indent) << "Variable(" << node.name << ")\n";
}

// Print a UnaryOpNode
inline void printNode(const UnaryOpNode& node, int indent) {
    std::cout << getIndent(indent) << "UnaryOp(" << TokenTypeToString(node.op) << "):\n";

    // Print the operand (which is a variant of numberNode or variableNode)
    std::visit([indent](const auto& operand) { printNode(operand, indent + 1); }, node.operand);
}

// Print a binaryOpNode
inline void printNode(const binaryOpNode& node, int indent) {
    std::cout << getIndent(indent) << "BinaryOp(" << TokenTypeToString(node.op) << "):\n";

    // Print left child
    printExpr(node.left, indent + 1);

    // Print right child
    printExpr(node.right, indent + 1);
}

// Main function to print any Expr (handles the variant)
inline void printExpr(const Expr& expr, int indent) {
    std::visit(
        [indent](const auto& node) {
            using T = std::decay_t<decltype(node)>;

            if constexpr (std::is_same_v<T, std::unique_ptr<binaryOpNode>>) {
                // Dereference the unique_ptr to get the actual binaryOpNode
                if (node) {
                    printNode(*node, indent);
                } else {
                    std::cout << getIndent(indent) << "NULL BinaryOpNode\n";
                }
            } else {
                // For numberNode, variableNode, and UnaryOpNode (not wrapped in
                // unique_ptr)
                printNode(node, indent);
            }
        },
        expr);
}

// Public API function - use this to print the result of Parser::parse()
inline void print_ast(const Expr& expr, int indent = 0) {
    printExpr(expr, indent);
}

#endif  // AST_PRINTER_H
