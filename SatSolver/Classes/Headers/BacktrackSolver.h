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
    unsigned long long getNumBacktracks() const { return num_backtracks; }
    unsigned long long getNumUnitPropagations() const { return num_unit_propagations; }
    unsigned long long getNumDecisions() const { return num_decisions; }

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

    unsigned long long num_backtracks = 0;
    unsigned long long num_unit_propagations = 0;
    unsigned long long num_decisions = 0;
};
#endif
