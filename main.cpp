#include "helpers.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <strings.h>
#include <unistd.h>
#include <vector>

bool intera = false;

std::string ReadFile(const std::filesystem::path& path) {
    auto&& file = std::ifstream(path);
    auto&& buffer = std::stringstream();
    buffer << file.rdbuf();
    return buffer.str();
}

void parse(std::string eq, bool dump, bool isVerbose) {
    Lexer lexer = Lexer(eq);
    std::vector<Token> tokens = lexer.tokenize();
    if (isVerbose)
        printContainer(tokens);
    (Parser::parse(tokens));
}

void interactive() {
    while (true) {
        std::string equation;
        std::cout << "\n>>>";
        std::getline(std::cin >> std::ws, equation);
        parse(equation, 1, 1);
    }
}

void printHelp() {
    std::cout << R"(Usage: ./bruuh -<OPTIONS> FILE

Options:
 -d    Dumps lexer output to output.lex
 -h    Shows this message
)";
}

int main(int argc, char* argv[]) {
    std::string equation;

    bool dumpLexer = false;
    bool verbose = false;
    std::string filename;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        bool intera = false;
        if (arg == "-d") {
            dumpLexer = true;
        } else if (arg == "-h") {
            printHelp();
        } else if (arg == "-v") {
            verbose = true;
        } else if (arg == "-i") {
            intera = true;
        }
    }

    if (intera) {
        interactive();
    } else {
        filename = "examples/ex2.bruh";  // argv[argc - 1];
        if (filename.empty()) {
            std::cerr << "Error: No input file specified\n";
            return 1;
        }

        equation = ReadFile(filename);
        parse(equation, dumpLexer, verbose);
    }
}
