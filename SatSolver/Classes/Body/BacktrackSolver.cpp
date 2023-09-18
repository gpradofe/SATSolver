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

bool BacktrackSolver::backtrack(size_t variable_index) {
    if (variable_index == current_assignment.size()) {
        return isSatisfied();
    }

    // Unit Propagation
    bool propagated;
    do {
        propagated = false;
        for (const Clause& clause : formula.getClauses()) {
            int unassigned_count = 0;
            int unassigned_literal = 0;
            bool clause_satisfied = false;

            for (const Literal& lit : clause.getLiterals()) {
                if (current_assignment[lit.getVariable() - 1] == BoolValue::UNASSIGNED) {
                    unassigned_count++;
                    unassigned_literal = lit.getVariable();
                } else if ((current_assignment[lit.getVariable() - 1] == BoolValue::TRUE && lit.getValue() == BoolValue::TRUE) ||
                           (current_assignment[lit.getVariable() - 1] == BoolValue::FALSE && lit.getValue() != BoolValue::TRUE)) {
                    clause_satisfied = true;
                    break;
                }
            }

            if (!clause_satisfied && unassigned_count == 1) {
                Literal last_unassigned_literal = clause.getLiterals()[unassigned_literal - 1];
                current_assignment[unassigned_literal - 1] = (last_unassigned_literal.getValue() == BoolValue::TRUE) ? BoolValue::TRUE : BoolValue::FALSE;
                propagated = true;
                std::cout << "Unit propagation: Assigning variable " << unassigned_literal << " to " << static_cast<int>(current_assignment[unassigned_literal - 1]) << std::endl;
            }
        }
    } while (propagated);


    int next_var = selectVariable();
    if (next_var == -1) return isSatisfied();

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
    std::vector<int> unsatisfied_counts(current_assignment.size(), 0);

    for (const Clause& clause : formula.getClauses()) {
        if (clause.evaluate(current_assignment) == BoolValue::FALSE) {
            for (const Literal& lit : clause.getLiterals()) {
                if (current_assignment[lit.getVariable() - 1] == BoolValue::UNASSIGNED) {
                    unsatisfied_counts[lit.getVariable() - 1]++;
                }
            }
        }
    }

    int max_index = std::distance(unsatisfied_counts.begin(), std::max_element(unsatisfied_counts.begin(), unsatisfied_counts.end()));
    if (unsatisfied_counts[max_index] > 0) {
        std::cout << "Selecting variable " << max_index + 1 << " with MOUSC heuristic." << std::endl;
        return max_index;
    }

    for (size_t i = 0; i < current_assignment.size(); ++i) {
        if (current_assignment[i] == BoolValue::UNASSIGNED) {
            std::cout << "Selecting variable " << i + 1 << " for assignment." << std::endl;
            return i;
        }
    }

    return -1;  // All variables are assigned
}
