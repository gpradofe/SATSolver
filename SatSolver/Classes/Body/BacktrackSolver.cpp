/**
<summary>
The BacktrackSolver class provides functionality to solve a Boolean formula
using the backtracking method with optimizations like unit propagation
and pure literal elimination.
</summary>
*/
#include "BacktrackSolver.h"

// Constructor for the BacktrackSolver class
BacktrackSolver::BacktrackSolver(BooleanFormula &formula)
    : formula(formula)
{
    // Initialize the current assignment vector with UNASSIGNED values
    int varCount = formula.getVariableCount();
    current_assignment.resize(varCount, BoolValue::UNASSIGNED);
    // Initialize the variable activity vector with zeros
    variable_activity.resize(varCount, 0);
}

/**
<summary>
Tries to solve the Boolean formula using backtracking.
</summary>
<returns>True if a solution is found, otherwise false.</returns>
*/
bool BacktrackSolver::solve()
{
    // Perform unit propagation and check for conflicts
    if (!unitPropagation())
    {
        return false;
    }
    // Perform pure literal elimination
    if (!pureLiteralElimination())
    {
        return false;
    }
    // Start the backtracking process
    return backtrack();
}

/**
<summary>
Gets the current assignment of variables.
</summary>
<returns>A vector of BoolValue representing the current variable assignments.</returns>
*/
const std::vector<BoolValue> &BacktrackSolver::getAssignment() const
{
    return current_assignment;
}

/**
<summary>
Recursive method that attempts to assign values to the variables and
checks if the current assignment satisfies the formula.
</summary>
<returns>True if a solution is found, otherwise false.</returns>
*/
bool BacktrackSolver::backtrack()
{
    // Increment the decision count
    num_decisions++;

    // Check if all clauses are satisfied with the current assignment
    if (isAllClausesSatisfied())
    {
        // Print the solution if all clauses are satisfied
        std::cout << "Solution found: ";
        for (BoolValue val : current_assignment)
        {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;
        return true;
    }

    // Decide which variable to assign next based on variable activity
    int variable_index = decideVariable();

    // No unassigned variable remains
    if (variable_index == -1)
    {
        return false;
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

    // If neither assignment worked, backtrack and mark the variable as unassigned
    current_assignment[variable_index] = BoolValue::UNASSIGNED;
    return false;
}

/**
<summary>
Decides which variable to try next based on the activity of variables
in unsatisfied clauses.
</summary>
<returns>The index of the variable to assign next. Returns -1 if no variable is found.</returns>
*/
int BacktrackSolver::decideVariable()
{
    // Reset the variable activity count
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

    // Find an unassigned variable with the highest frequency (activity)
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

/**
<summary>
Checks if the current assignment satisfies the formula.
</summary>
<returns>True if the formula is satisfied, otherwise false.</returns>
*/
bool BacktrackSolver::isSatisfied()
{
    for (const Clause &clause : formula.getClauses())
    {
        if (clause.evaluate(current_assignment) == BoolValue::FALSE)
        {
            // Increment variable activity for the literals in unsatisfied clauses
            for (const Literal &lit : clause.getLiterals())
            {
                variable_activity[lit.getVariable() - 1]++;
            }
            return false;
        }
    }
    return true;
}

/**
<summary>
Checks if all the clauses in the formula are satisfied with the current assignment.
</summary>
<returns>True if all clauses are satisfied, otherwise false.</returns>
*/
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

/**
<summary>
Performs unit propagation on the formula. It iteratively assigns values to
literals that are the only unassigned literal in their clause.
</summary>
<returns>True if no conflicts are detected, otherwise false.</returns>
*/
bool BacktrackSolver::unitPropagation()
{
    bool change = true; // To keep track if any assignment was done in the last iteration
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
                    // The clause is already satisfied, no need to check further
                    unassignedCount = -1;
                    break;
                }
            }

            // Check if the clause is a unit clause
            if (unassignedCount == 1)
            {
                num_unit_propagations++;
                BoolValue assignedVal = unassignedLiteral.getValue();
                if (current_assignment[unassignedLiteral.getVariable() - 1] == BoolValue::UNASSIGNED)
                {
                    current_assignment[unassignedLiteral.getVariable() - 1] = assignedVal;
                    change = true; // Mark change as true since an assignment was made
                }
            }
        }
    }
    return true; // No conflicts detected
}

/**
<summary>
Performs pure literal elimination on the formula. It identifies and assigns
values to literals that appear in only one polarity (either positive or negative)
across all clauses.
</summary>
<returns>True if the operation is successful.</returns>
*/
bool BacktrackSolver::pureLiteralElimination()
{
    // Create vectors to track the presence of positive and negative literals
    std::vector<bool> positiveSeen(formula.getVariableCount(), false);
    std::vector<bool> negativeSeen(formula.getVariableCount(), false);

    // Iterate through clauses and mark the presence of literals
    for (const Clause &clause : formula.getClauses())
    {
        for (const Literal &literal : clause.getLiterals())
        {
            int var = abs(literal.getVariable()) - 1; // Adjust for 0-based indexing
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

    // Create sets to store pure positive and pure negative literals
    std::set<int> purePositives;
    std::set<int> pureNegatives;

    // Identify pure literals and insert them into the corresponding sets
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

    // Assign TRUE to pure positive literals and FALSE to pure negative literals
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
