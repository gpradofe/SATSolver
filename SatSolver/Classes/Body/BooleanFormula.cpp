#include "BooleanFormula.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
void BooleanFormula::addClause(const Clause &clause)
{
    clauses.push_back(clause);
}

std::vector<BooleanFormula> BooleanFormula::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open the file");
    }

    std::vector<BooleanFormula> formulas;
    std::string line;

    BooleanFormula current_formula;
    bool reading_formula = false;

    while (std::getline(file, line))
    {
        std::cout << "Reading line: " << line << std::endl;

        if (line[0] == 'c')
        {
            if (reading_formula)
            {
                formulas.push_back(current_formula);
                current_formula = BooleanFormula();
            }
            reading_formula = true;
            continue;
        }

        if (line[0] == 'p')
        {
            continue;
        }

        if (reading_formula)
        {
            std::istringstream iss(line);
            std::string literal_str;
            Clause clause;

            while (std::getline(iss, literal_str, ','))
            { // Using ',' as the delimiter
                int literal_val = std::stoi(literal_str);

                if (literal_val == 0)
                {
                    current_formula.addClause(clause);
                    std::cout << "Added clause: ";
                    for (const Literal &lit : clause.getLiterals())
                    {
                        std::cout << (lit.getValue() == BoolValue::TRUE ? "" : "-") << lit.getVariable() << " ";
                    }
                    std::cout << std::endl;

                    clause.clear();
                    break;
                }

                BoolValue literal_bool = (literal_val > 0) ? BoolValue::TRUE : BoolValue::FALSE;
                int variable = std::abs(literal_val);
                Literal literal(variable, literal_bool);

                // Debug print for each literal
                std::cout << "Parsed Literal: " << (literal.getValue() == BoolValue::TRUE ? "" : "-") << literal.getVariable() << std::endl;

                clause.addLiteral(literal);
            }
        }
    }

    if (reading_formula)
    {
        formulas.push_back(current_formula);
    }

    file.close();
    return formulas;
}

int BooleanFormula::getVariableCount() const
{
    int max_variable = 0;
    for (const Clause &clause : clauses)
    {
        for (const Literal &lit : clause.getLiterals())
        {
            max_variable = std::max(max_variable, lit.getVariable());
        }
    }
    std::cout << "Max variable for this formula: " << max_variable << std::endl;
    return max_variable;
}

const std::vector<Clause> &BooleanFormula::getClauses() const
{
    return clauses;
}
void BooleanFormula::removeRedundantClauses(const std::vector<BoolValue> &assignment)
{
    clauses.erase(std::remove_if(clauses.begin(), clauses.end(),
                                 [&](const Clause &c)
                                 { return c.evaluate(assignment) == BoolValue::TRUE; }),
                  clauses.end());
}
std::vector<Clause> &BooleanFormula::getClausesDirect()
{
    return clauses;
}