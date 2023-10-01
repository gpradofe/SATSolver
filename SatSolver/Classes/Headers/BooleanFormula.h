#pragma once
#include "Clause.h"
#include <vector>

class BooleanFormula
{
public:
    /**
    <summary>
    Adds a clause to the current formula.
    </summary>
    <param name="clause">The clause to be added.</param>
    */
    void addClause(const Clause &clause);

    /**
    <summary>
    Evaluates the formula with the given variable assignments.
    </summary>
    <param name="assignments">A vector of BoolValue representing variable assignments.</param>
    <returns>The evaluation result as BoolValue (TRUE, FALSE, or UNASSIGNED).</returns>
    */
    BoolValue evaluate(const std::vector<BoolValue> &assignments) const;

    /**
    <summary>
    Loads a collection of Boolean formulas from a file.
    </summary>
    <param name="filename">The name of the file to be read.</param>
    <returns>A vector of Boolean formulas read from the file.</returns>
    */
    std::vector<BooleanFormula> loadFromFile(const std::string &filename);

    /**
    <summary>
    Retrieves the number of variables in the formula.
    </summary>
    <returns>The count of variables in the formula.</returns>
    */
    int getVariableCount() const;

    /**
    <summary>
    Gets the maximum number of literals in any single clause of the formula.
    </summary>
    <returns>The maximum number of literals in a clause.</returns>
    */
    int getMaxLiteralsInClause() const;

    /**
    <summary>
    Counts the total number of literals across all clauses in the formula.
    </summary>
    <returns>The total count of literals.</returns>
    */
    int getTotalLiterals() const;

    /**
    <summary>
    Gets the clauses in the formula.
    </summary>
    <returns>A constant reference to the list of clauses in the formula.</returns>
    */
    const std::vector<Clause> &getClauses() const;

    /**
    <summary>
    Removes clauses that are made redundant by a given assignment.
    </summary>
    <param name="assignment">The assignment of values to variables.</param>
    */
    void removeRedundantClauses(const std::vector<BoolValue> &assignment);

    /**
    <summary>
    Provides direct access to the clauses in the formula.
    </summary>
    <returns>A reference to the list of clauses in the formula.</returns>
    <remarks>
    Use with caution as this allows for direct modification of the clause list.
    </remarks>
    */
    std::vector<Clause> &getClausesDirect();

    /**
    <summary>
    Gets the answer associated with the formula.
    </summary>
    <returns>The answer as a character ('T' for True, 'F' for False, 'U' for Unknown).</returns>
    */
    char getAnswer() const { return answer; }

    /**
    <summary>
    Sets the answer associated with the formula.
    </summary>
    <param name="ans">The answer to set as a character ('T' for True, 'F' for False, 'U' for Unknown).</param>
    */
    void setAnswer(char ans) { answer = ans; }

    /**
    <summary>
    Retrieves the number of clauses in the formula.
    </summary>
    <returns>The count of clauses in the formula.</returns>
    */
    int getClauseCount() const { return clauses.size(); };

private:
    std::vector<Clause> clauses;
    char answer;
};
