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
    BacktrackSolver(BooleanFormula &formula);

    bool solve();
    const std::vector<BoolValue> &getAssignment() const;

private:
    BooleanFormula &formula;
    std::vector<BoolValue> current_assignment;
    std::vector<int> variable_activity; // <-- Add this line

    bool backtrack();
    bool isSatisfied();
    bool unitPropagation(); // New method for unit propagation
    bool pureLiteralElimination();
    int decideVariable();
    bool isAllClausesSatisfied();
};
#endif
