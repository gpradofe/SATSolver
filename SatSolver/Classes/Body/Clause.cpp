#include "Clause.h"

/**
<summary>
Adds a literal to the clause.
</summary>
<param name="literal">The literal to be added.</param>
*/
void Clause::addLiteral(const Literal &literal)
{
    literals.push_back(literal);
}

/**
<summary>
Evaluates the clause given a set of assignments.
</summary>
<param name="assignments">The assignments of values to literals.</param>
<returns>
Returns BoolValue::TRUE if any literal evaluates to true given the assignments, otherwise returns BoolValue::FALSE.
</returns>
<remarks>
The method will check each literal's evaluation and if any of them is true, the entire clause is true.
</remarks>
*/
BoolValue Clause::evaluate(const std::vector<BoolValue> &assignments) const
{
    for (const Literal &lit : literals)
    {
        if (lit.evaluate(assignments[abs(lit.getVariable()) - 1]) == BoolValue::TRUE)
        {
            return BoolValue::TRUE;
        }
    }

    return BoolValue::FALSE;
}

/**
<summary>
Retrieves the literals in the clause.
</summary>
<returns>A constant reference to the list of literals in the clause.</returns>
*/
const std::vector<Literal> &Clause::getLiterals() const
{
    return literals;
}

/**
<summary>
Clears all literals from the clause.
</summary>
*/
void Clause::clear()
{
    literals.clear();
}

/**
<summary>
Constructor for the Clause class that initializes the clause with a set of literals.
</summary>
<param name="literals">A vector of literals to initialize the clause with.</param>
*/
Clause::Clause(const std::vector<Literal> &literals) : literals(literals) {}
