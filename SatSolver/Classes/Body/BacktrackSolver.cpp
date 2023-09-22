#include "BacktrackSolver.h"

BacktrackSolver::BacktrackSolver(BooleanFormula& formula) 
    : formula(formula) {
    current_assignment.resize(formula.getVariableCount(), BoolValue::UNASSIGNED);
}

bool BacktrackSolver::solve() {
    if (!unitPropagation()) {
        return false;  // Conflict detected
    }
    return backtrack();
}

const std::vector<BoolValue>& BacktrackSolver::getAssignment() const {
    return current_assignment;
}

bool BacktrackSolver::backtrack() {
    size_t variable_index = mostConstrainedVariable();

    // If all variables are assigned, check satisfaction
    if (current_assignment[variable_index] != BoolValue::UNASSIGNED) {
        return isSatisfied();
    }

    current_assignment[variable_index] = BoolValue::TRUE;
    if (backtrack()) {
        return true;
    }

    current_assignment[variable_index] = BoolValue::FALSE;
    if (backtrack()) {
        return true;
    }

    current_assignment[variable_index] = BoolValue::UNASSIGNED;  // Reset the variable to unassigned
    return false;
}

size_t BacktrackSolver::mostConstrainedVariable() {
    std::vector<int> constraintCount(current_assignment.size(), 0);

    for (const Clause& clause : formula.getClauses()) {
        // If clause is already satisfied, skip it.
        if (clause.evaluate(current_assignment) == BoolValue::TRUE) continue;

        for (const Literal& literal : clause.getLiterals()) {
            if (current_assignment[literal.getVariable() - 1] == BoolValue::UNASSIGNED) {
                constraintCount[literal.getVariable() - 1]++;
            }
        }
    }

    size_t maxIndex = 0;
    int maxCount = -1;
    for (size_t i = 0; i < constraintCount.size(); i++) {
        if (constraintCount[i] > maxCount) {
            maxCount = constraintCount[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

bool BacktrackSolver::isSatisfied() const {
    for (const Clause& clause : formula.getClauses()) {
        if (clause.evaluate(current_assignment) == BoolValue::FALSE) {
            return false;
        }
    }
    return true;
}


bool BacktrackSolver::unitPropagation() {
    bool change = true;  // to keep track if any assignment was done in the last iteration
    while (change) {
        change = false;

        for (const Clause& clause : formula.getClauses()) {
            int unassignedCount = 0;
            Literal unassignedLiteral;

            for (const Literal& literal : clause.getLiterals()) {
                BoolValue val = current_assignment[literal.getVariable() - 1];
                if (val == BoolValue::UNASSIGNED) {
                    unassignedCount++;
                    unassignedLiteral = literal;
                } else if (literal.evaluate(val) == BoolValue::TRUE) {
                    unassignedCount = -1;  // clause is already satisfied, no need to check further
                    break;
                }
            }

            // Check if clause is a unit clause
            if (unassignedCount == 1) {
                BoolValue assignedVal = (unassignedLiteral.getValue() == BoolValue::TRUE) ? BoolValue::TRUE : BoolValue::FALSE;

                if (current_assignment[unassignedLiteral.getVariable() - 1] == BoolValue::UNASSIGNED) {
                    current_assignment[unassignedLiteral.getVariable() - 1] = assignedVal;
                    change = true;
                } else if (current_assignment[unassignedLiteral.getVariable() - 1] != assignedVal) {
                    return false;  // Conflict detected
                }
            }
        }
    }
    return true;  // No conflicts detected
}


