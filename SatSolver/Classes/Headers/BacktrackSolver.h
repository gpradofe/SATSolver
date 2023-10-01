#ifndef BACKTRACKSOLVER_H
#define BACKTRACKSOLVER_H

#include "BooleanFormula.h"
#include "Clause.h"
#include "Literal.h"
#include <vector>
#include <iostream>
#include <set>
#include <cstdlib>
#include <algorithm>

class BacktrackSolver
{
public:
    /**
    <summary>
    Constructor for the BacktrackSolver class.
    </summary>
    <param name="formula">The Boolean formula to be solved.</param>
    */
    BacktrackSolver(BooleanFormula &formula);

    /**
    <summary>
    Attempts to solve the Boolean formula using backtracking.
    </summary>
    <returns>True if a solution is found, otherwise false.</returns>
    */
    bool solve();

    /**
    <summary>
    Gets the current assignment of variables.
    </summary>
    <returns>A vector of BoolValue representing the current variable assignments.</returns>
    */
    const std::vector<BoolValue> &getAssignment() const;

    /**
    <summary>
    Gets the number of backtracks performed during solving.
    </summary>
    <returns>The number of backtracks.</returns>
    */
    unsigned long long getNumBacktracks() const { return num_backtracks; }

    /**
    <summary>
    Gets the number of unit propagations performed during solving.
    </summary>
    <returns>The number of unit propagations.</returns>
    */
    unsigned long long getNumUnitPropagations() const { return num_unit_propagations; }

    /**
    <summary>
    Gets the number of decisions made during solving.
    </summary>
    <returns>The number of decisions.</returns>
    */
    unsigned long long getNumDecisions() const { return num_decisions; }

private:
    BooleanFormula &formula;
    std::vector<BoolValue> current_assignment;
    std::vector<int> variable_activity; // Activity of variables for decision order

    /**
    <summary>
    Recursive method that attempts to assign values to the variables and
    checks if the current assignment satisfies the formula.
    </summary>
    <returns>True if a solution is found, otherwise false.</returns>
    */
    bool backtrack();

    /**
    <summary>
    Checks if the current assignment satisfies the formula.
    </summary>
    <returns>True if the formula is satisfied, otherwise false.</returns>
    */
    bool isSatisfied();

    /**
    <summary>
    Performs unit propagation on the formula. It iteratively assigns values to
    literals that are the only unassigned literal in their clause.
    </summary>
    <returns>True if no conflicts are detected, otherwise false.</returns>
    */
    bool unitPropagation();

    /**
    <summary>
    Performs pure literal elimination on the formula. It identifies and assigns
    values to literals that appear in only one polarity (either positive or negative)
    across all clauses.
    </summary>
    <returns>True if the operation is successful.</returns>
    */
    bool pureLiteralElimination();

    /**
    <summary>
    Decides which variable to try next based on the activity of variables
    in unsatisfied clauses.
    </summary>
    <returns>The index of the variable to assign next. Returns -1 if no variable is found.</returns>
    */
    int decideVariable();

    /**
    <summary>
    Checks if all the clauses in the formula are satisfied with the current assignment.
    </summary>
    <returns>True if all clauses are satisfied, otherwise false.</returns>
    */
    bool isAllClausesSatisfied();

    unsigned long long num_backtracks = 0;
    unsigned long long num_unit_propagations = 0;
    unsigned long long num_decisions = 0;
};

#endif
