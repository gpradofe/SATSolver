#pragma once
#include "BooleanFormula.h"

class BacktrackSolver {
public:
    BacktrackSolver(const BooleanFormula& formula);

    // Find a satisfying assignment using backtracking
    bool solve();

    // Get the satisfying assignment (if found)
    const std::vector<BoolValue>& getAssignment() const;

private:
    const BooleanFormula& formula;
    std::vector<BoolValue> current_assignment;

    // Recursive backtracking function
    bool backtrack(int variable_index);

    // Check if the current assignment satisfies the formula
    bool isSatisfied() const;

    // Select the next unassigned variable for branching
    int selectVariable() const;
};
