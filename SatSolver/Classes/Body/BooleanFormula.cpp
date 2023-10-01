#include "BooleanFormula.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

/**
<summary>
Adds a clause to the current formula.
</summary>
<param name="clause">The clause to be added.</param>
*/
void BooleanFormula::addClause(const Clause &clause)
{
    clauses.push_back(clause);
}

/**
<summary>
Loads a collection of Boolean formulas from a file.
</summary>
<param name="filename">The name of the file to be read.</param>
<returns>A vector of Boolean formulas read from the file.</returns>
<remarks>
The file format is expected to contain meta information lines starting with 'c',
formula specification lines starting with 'p', and clauses.
</remarks>
*/
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

        // Check if the line contains a comment (starts with 'c')
        if (line[0] == 'c')
        {
            if (reading_formula)
            {
                formulas.push_back(current_formula);
                current_formula = BooleanFormula();
            }

            // Determine the delimiter used in the comment line
            char delimiter = (line.find(',') != std::string::npos) ? ',' : ' ';

            // Extract the answer if it exists
            std::istringstream iss(line);
            std::string token;
            std::getline(iss, token, delimiter); // This reads the 'c' character
            std::getline(iss, token, delimiter); // Skip the formula number
            std::getline(iss, token, delimiter); // Skip the variable count

            if (iss >> token)
            {
                current_formula.setAnswer(token[0]);
            }
            else
            {
                current_formula.setAnswer('U'); // Default to 'U' if not provided
            }

            reading_formula = true;
            continue;
        }

        // Check if the line contains the formula specification (starts with 'p')
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

/**
<summary>
Retrieves the number of variables in the formula.
</summary>
<returns>The count of variables in the formula.</returns>
*/
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
    return max_variable;
}

/**
<summary>
Gets the clauses in the formula.
</summary>
<returns>A constant reference to the list of clauses in the formula.</returns>
*/
const std::vector<Clause> &BooleanFormula::getClauses() const
{
    return clauses;
}

/**
<summary>
Removes clauses that are made redundant by a given assignment.
</summary>
<param name="assignment">The assignment of values to variables.</param>
<remarks>
A clause is considered redundant if its evaluation with the assignment results in TRUE.
</remarks>
*/
void BooleanFormula::removeRedundantClauses(const std::vector<BoolValue> &assignment)
{
    clauses.erase(std::remove_if(clauses.begin(), clauses.end(),
                                 [&](const Clause &c)
                                 { return c.evaluate(assignment) == BoolValue::TRUE; }),
                  clauses.end());
}

/**
<summary>
Provides direct access to the clauses in the formula.
</summary>
<returns>A reference to the list of clauses in the formula.</returns>
<remarks>
Use with caution as this allows for direct modification of the clause list.
</remarks>
*/
std::vector<Clause> &BooleanFormula::getClausesDirect()
{
    return clauses;
}

/**
<summary>
Determines the maximum number of literals in any single clause of the formula.
</summary>
<returns>The maximum number of literals in a clause.</returns>
*/
int BooleanFormula::getMaxLiteralsInClause() const
{
    int max_literals = 0;
    for (const Clause &clause : clauses)
    {
        max_literals = std::max(max_literals, static_cast<int>(clause.getLiterals().size()));
    }
    return max_literals;
}

/**
<summary>
Counts the total number of literals across all clauses in the formula.
</summary>
<returns>The total count of literals.</returns>
*/
int BooleanFormula::getTotalLiterals() const
{
    int total = 0;
    for (const Clause &clause : clauses)
    {
        total += clause.getLiterals().size();
    }
    return total;
}
