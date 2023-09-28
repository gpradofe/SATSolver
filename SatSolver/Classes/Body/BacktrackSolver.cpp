#include "BacktrackSolver.h"

BacktrackSolver::BacktrackSolver(BooleanFormula &formula)
    : formula(formula)
{
    int varCount = formula.getVariableCount();
    current_assignment.resize(varCount, BoolValue::UNASSIGNED);
    variable_activity.resize(varCount, 0);
}

bool BacktrackSolver::solve()
{
    if (!unitPropagation())
    {
        return false;
    }
    if (!pureLiteralElimination())
    {
        return false;
    }
    return backtrack();
}

const std::vector<BoolValue> &BacktrackSolver::getAssignment() const
{
    return current_assignment;
}

bool BacktrackSolver::backtrack()
{
    num_decisions++;
    if (isAllClausesSatisfied())
    {
        std::cout << "Solution found: ";
        for (BoolValue val : current_assignment)
        {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;
        return true;
    }

    int variable_index = decideVariable();

    if (variable_index == -1)
    {
        return false; // No unassigned variable
    }

    // Try TRUE assignment
    current_assignment[variable_index] = BoolValue::TRUE;
    num_backtracks++;
    if (backtrack())
    {
        return true;
    }
    // Try FALSE assignment
    current_assignment[variable_index] = BoolValue::FALSE;
    if (backtrack())
    {
        return true;
    }

    // If neither assignment worked, backtrack
    current_assignment[variable_index] = BoolValue::UNASSIGNED;
    return false;
}

int BacktrackSolver::decideVariable()
{
    // Reset the variable_activity count
    std::fill(variable_activity.begin(), variable_activity.end(), 0);

    // Count the frequency of variables in unsatisfied clauses
    for (const Clause &clause : formula.getClauses())
    {
        if (clause.evaluate(current_assignment) == BoolValue::FALSE)
        {
            for (const Literal &lit : clause.getLiterals())
            {
                variable_activity[lit.getVariable() - 1]++;
            }
        }
    }

    // Find unassigned variable with highest frequency
    int max_frequency = -1;
    int chosen_variable = -1;
    for (size_t i = 0; i < current_assignment.size(); i++)
    {
        if (current_assignment[i] == BoolValue::UNASSIGNED && variable_activity[i] > max_frequency)
        {
            max_frequency = variable_activity[i];
            chosen_variable = i;
        }
    }

    return (chosen_variable != -1) ? chosen_variable : -1;
}

bool BacktrackSolver::isSatisfied()
{
    for (const Clause &clause : formula.getClauses())
    {
        if (clause.evaluate(current_assignment) == BoolValue::FALSE)
        {
            for (const Literal &lit : clause.getLiterals())
            {
                variable_activity[lit.getVariable() - 1]++;
            }
            return false;
        }
    }
    return true;
}
bool BacktrackSolver::isAllClausesSatisfied()
{
    for (const Clause &clause : formula.getClauses())
    {
        if (clause.evaluate(current_assignment) == BoolValue::FALSE)
        {
            return false;
        }
    }
    return true;
}

bool BacktrackSolver::unitPropagation()
{
    bool change = true; // to keep track if any assignment was done in the last iteration
    while (change)
    {
        change = false;

        for (const Clause &clause : formula.getClauses())
        {
            int unassignedCount = 0;
            Literal unassignedLiteral;

            for (const Literal &literal : clause.getLiterals())
            {
                BoolValue val = current_assignment[literal.getVariable() - 1];
                if (val == BoolValue::UNASSIGNED)
                {
                    unassignedCount++;
                    unassignedLiteral = literal;
                }
                else if (literal.evaluate(val) == BoolValue::TRUE)
                {
                    unassignedCount = -1; // clause is already satisfied, no need to check further
                    break;
                }
            }

            // Check if clause is a unit clause
            if (unassignedCount == 1)
            {
                num_unit_propagations++;
                BoolValue assignedVal = unassignedLiteral.getValue();
                if (current_assignment[unassignedLiteral.getVariable() - 1] == BoolValue::UNASSIGNED)
                {
                    current_assignment[unassignedLiteral.getVariable() - 1] = assignedVal;
                    change = true; // Moved this inside the if statement.
                }
            }
        }
    }
    return true; // No conflicts detected
}
bool BacktrackSolver::pureLiteralElimination()
{
    std::vector<bool> positiveSeen(formula.getVariableCount(), false);
    std::vector<bool> negativeSeen(formula.getVariableCount(), false);

    for (const Clause &clause : formula.getClauses())
    {
        for (const Literal &literal : clause.getLiterals())
        {
            int var = abs(literal.getVariable()) - 1; // adjust for 0-based indexing
            if (literal.getValue() == BoolValue::TRUE)
            {
                positiveSeen[var] = true;
            }
            else
            {
                negativeSeen[var] = true;
            }
        }
    }

    std::set<int> purePositives;
    std::set<int> pureNegatives;

    for (size_t i = 0; i < positiveSeen.size(); i++)
    {
        if (positiveSeen[i] && !negativeSeen[i])
        {
            purePositives.insert(i + 1);
        }
        else if (!positiveSeen[i] && negativeSeen[i])
        {
            pureNegatives.insert(i + 1);
        }
    }

    for (int var : purePositives)
    {
        current_assignment[var - 1] = BoolValue::TRUE;
    }

    for (int var : pureNegatives)
    {
        current_assignment[var - 1] = BoolValue::FALSE;
    }

    return true;
}
