#include "BacktrackSolver.h"
#include <iostream>
BacktrackSolver::BacktrackSolver(const BooleanFormula& formula) : formula(formula) {
    current_assignment.resize(formula.getVariableCount(), BoolValue::UNASSIGNED);
}

bool BacktrackSolver::solve() {
    std::cout << "Starting backtracking process." << std::endl;
    return backtrack(0);
}

const std::vector<BoolValue>& BacktrackSolver::getAssignment() const {
    return current_assignment;
}

bool BacktrackSolver::backtrack(int variable_index) {
    if (variable_index == current_assignment.size()) {
        return isSatisfied();
    }

    // Unit propagation and pure literal elimination can be implemented here

    int next_var = selectVariable();

    current_assignment[next_var] = BoolValue::TRUE;
    if (backtrack(variable_index + 1)) {
        return true;
    }

    current_assignment[next_var] = BoolValue::FALSE;
    if (backtrack(variable_index + 1)) {
        return true;
    }

    std::cout << "Backtracking on variable " << next_var + 1 << std::endl;
    current_assignment[next_var] = BoolValue::UNASSIGNED;
    return false;
}

bool BacktrackSolver::isSatisfied() const {
    bool result = formula.evaluate(current_assignment) == BoolValue::TRUE;
    std::cout << "Evaluating assignment: ";
    for (BoolValue val : current_assignment) {
        std::cout << static_cast<int>(val) << " ";
    }
    std::cout << "\\nEvaluation result: " << (result ? "SATISFIED" : "NOT SATISFIED") << std::endl;
    return result;
}

int BacktrackSolver::selectVariable() const {
    for (size_t i = 0; i < current_assignment.size(); ++i) {
        if (current_assignment[i] == BoolValue::UNASSIGNED) {
            std::cout << "Selecting variable " << i + 1 << " for assignment." << std::endl;
            return i;  // Return the first unassigned variable
        }
    }
    return -1;  // All variables are assigned
}
