#include <strings.h>
#include <algorithm>
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
    ConicSolver(std::string equation)
    {
			m_equation = lower(equation);
        if (!(in(equation, "x^")) || !(in(equation, "y^")))
        {
            m_Problem = Parabola;
						printContainer(Lex(equation));
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

int main()
{
    std::string equation;
    std::cout << "you can input the equation using these "
                 "symbols:\n\n\t+\t:addition\n\t-\t:subtraction\n\t*\t:"
                 "multiplication\n\t^\t:power\n\n";
    // std::cout << "Equation: ";
    //  std::cin >> equation;
    equation  = "123 + 5";// "3*y^2-8y+3x+100";  //"3y^2 + 8y + 3x + 100";
    ConicSolver Solver(equation);
}
