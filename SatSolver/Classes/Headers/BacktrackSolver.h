#ifndef BACKTRACKSOLVER_H
#define BACKTRACKSOLVER_H

#include "BooleanFormula.h"
#include "Clause.h"
#include "Literal.h"
#include <vector>
#include <iostream>

class BacktrackSolver {
public:
    BacktrackSolver(BooleanFormula& formula);

    bool solve();
    const std::vector<BoolValue>& getAssignment() const;

private:
    BooleanFormula& formula;
    std::vector<BoolValue> current_assignment;
    size_t mostConstrainedVariable();
    bool backtrack();
    bool isSatisfied() const;
    bool unitPropagation(); // New method for unit propagation
};

#endif
