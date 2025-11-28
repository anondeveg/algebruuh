#include <bits/stdc++.h>

#include <cctype>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "internals.h"
bool isDigitString(std::string str)
{
    for (char x : str)
    {
        if (!(std::isdigit(x)))
        {
            return false;
        }
    }
    return true;
}
int getTermVariableIndex(std::string strTerm, int index = 0)
{
    while (isdigit(strTerm[index]))
    {
        index++;
    }
    return index;
}

std::vector<std::string> split(std::string base, char delim)
{
    std::stringstream ss(base);
    std::string temp;
    std::vector<std::string> v;
    while (std::getline(ss, temp, delim))
    {
        v.emplace_back(temp);
    }

    printContainer(v);
    return v;
}

TokenTypes getType(char x)
{
    if (std::isdigit(x))
    {
        return NUM;
    }
    else
    {
        return VAR;
    }
}
std::tuple<Token, std::string> _lex(std::string tokenBuilder, std::string equation,
                                    TokenTypes last_type = NUL)
{
    char x = equation[0];
    if (x == ' ')
    {  // skip spaces

        return _lex(tokenBuilder, equation.substr(1, equation.size()), last_type);
    }

    if (last_type != getType(x) && last_type != NUL)
    {
        return make_tuple(Token{tokenBuilder, last_type}, equation);
    }
    switch (x)
    {
        case '+':
            return std::make_tuple(Token{(std::string() + x), ADDOP},
                                   equation.substr(1, equation.size()));

        case '-':
            return std::make_tuple(Token{(std::string() + x), SUBOP},
                                   equation.substr(1, equation.size()));
        case '*':
            return std::make_tuple(Token{(std::string() + x), MULOP},
                                   equation.substr(1, equation.size()));
        case '/':
            return std::make_tuple(Token{(std::string() + x), DIVOP},
                                   equation.substr(1, equation.size()));
        case '^':
            return std::make_tuple(Token{(std::string() + x), POWOP},
                                   equation.substr(1, equation.size()));
    }

    if (std::isdigit(x) && (last_type == NUM || last_type == NUL))
    {
        tokenBuilder += x;
        return _lex(tokenBuilder, equation.substr(1, equation.size()),
                    NUM);  // remove the first charachter
    }

    return std::make_tuple(Token{(std::string() + x), VAR}, equation.substr(1, equation.size()));
    std::string error = "ERROR\n x->" + (std::string() + x) + "\nEquation ->" + equation;

    throw error;
}

std::vector<Token> Lex(std::string equation, bool dump = false, bool verbose = false)
{
    std::vector<Token> Tokens;
    std::vector<Token> x;
    while (equation.size() != 0)
    {
        std::tuple<Token, std::string> y;

        y = _lex("", equation);
        if (verbose)
        {
            std::cout << "\n" << std::get<0>(y)<< "\n";
        }
        x.emplace_back(std::get<0>(y));

        equation = std::get<1>(y);
    }

    if (dump)
    {
        writeToFile("dump.lex", x);
				std::cout << "\n[TOKENS DUMPED] dump.lex\n";
    }

		return x;
}
