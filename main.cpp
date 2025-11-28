#include <strings.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <string>

#include "internals.h"
enum ProblemType
{
    Parabola
};
class ConicSolver
{
   public:
    ProblemType m_Problem = Parabola;
    std::string m_equation;
    ConicSolver(std::string equation, bool dump = false, bool verbose = false)
    {
        m_equation = lower(equation);
        if (!(in(equation, "x^")) || !(in(equation, "y^")))
        {
            m_Problem = Parabola;
            Lex(equation, dump, verbose);
        }
    }

    bool in(std::string base, std::string check)
    {
        bool isIn = base.find(check) != std::string::npos;  // npos is just -1
        return isIn;
    }

    std::string lower(std::string data)
    {
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        return data;
    }
};
std::string ReadFile(std::string filename)
{
    std::ifstream file(filename);
    std::string temp{};
    std::string code;
    if (file.is_open())
    {
        while (getline(file, temp))
        {
            code += temp;
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return code;
}

void interactive()
{
    while (true)
    {
        std::string equation;
        std::cout << "\n>>>";
        std::getline(std::cin >> std::ws, equation);
        ConicSolver Solver(equation, 0, 1);  // verbose
    }
}
void printHelp()
{
    std::cout << R"(Usage: ./bruuh -<OPTIONS> FILE

Options:
 -d    Dumps lexer output to output.lex
 -h    Shows this message
)";
}
int main(int argc, char* argv[])
{
    std::string equation;

    bool dumpLexer = false;
    bool verbose = false;
    std::string filename;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        bool intera = false;
        if (arg == "-d")
        {
            dumpLexer = true;
        }
        else if (arg == "-h")
        {
            printHelp();
        }
        else if (arg == "-v")
        {
            verbose = true;
        }
        else if (arg == "-i")
        {
            intera = true;
        }

        if (intera)
        {
            interactive();
        }
        else
        {
            filename = argv[argc - 1];
            if (filename.empty())
            {
                std::cerr << "Error: No input file specified\n";
                return 1;
            }

						equation = ReadFile(filename);
            ConicSolver Solver(equation, dumpLexer, verbose);
        }
    }
}
