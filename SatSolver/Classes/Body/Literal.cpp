#include "Literal.h"

/**
<summary>
Constructor for the Literal class that initializes a literal with a variable and a value.
</summary>
<param name="var">The variable associated with the literal.</param>
<param name="val">The value of the literal.</param>
*/
Literal::Literal(int var, BoolValue val) : variable(var), value(val) {}

/**
<summary>
Evaluates the literal given its assignment.
</summary>
<param name="assignment">The value assigned to the literal.</param>
<returns>
Returns the evaluation result of the literal given the assignment.
</returns>
<remarks>
If the literal's value is TRUE, it returns the assignment value directly.
If the literal's value is FALSE, it returns the negated value of the assignment.
For unassigned literals, it returns BoolValue::UNASSIGNED.
</remarks>
*/
BoolValue Literal::evaluate(BoolValue assignment) const
{
    if (value == BoolValue::TRUE)
    {
        return assignment;
    }
    else if (value == BoolValue::FALSE)
    {
        return (assignment == BoolValue::TRUE) ? BoolValue::FALSE : BoolValue::TRUE;
    }
    return BoolValue::UNASSIGNED; // This line may not be necessary if literals are always TRUE or FALSE.
}

/**
<summary>
Retrieves the variable of the literal.
</summary>
<returns>The variable associated with the literal.</returns>
*/
int Literal::getVariable() const
{
    return variable;
}

/**
<summary>
Retrieves the value of the literal.
</summary>
<returns>The value of the literal.</returns>
*/
BoolValue Literal::getValue() const
{
    return value;
}

/**
<summary>
Returns a new literal that is the negation of the current one.
</summary>
<returns>A new literal which is the negation of the current one.</returns>
<remarks>
For a TRUE value, the negation will be FALSE, and vice versa.
For unassigned literals, the negation remains UNASSIGNED.
</remarks>
*/
Literal Literal::negate() const
{
    if (value == BoolValue::TRUE)
    {
        return Literal(variable, BoolValue::FALSE);
    }
    else if (value == BoolValue::FALSE)
    {
        return Literal(variable, BoolValue::TRUE);
    }
    return Literal(variable, BoolValue::UNASSIGNED);
}

/**
<summary>
Compares two literals to determine their order.
</summary>
<param name="lhs">The left-hand side literal in the comparison.</param>
<param name="rhs">The right-hand side literal in the comparison.</param>
<returns>
Returns true if the lhs literal is "less than" the rhs literal, otherwise returns false.
</returns>
<remarks>
Literals are compared first by their variables and then by their values.
</remarks>
*/
bool operator<(const Literal &lhs, const Literal &rhs)
{
    if (lhs.getVariable() == rhs.getVariable())
    {
        return lhs.getValue() < rhs.getValue();
    }
    return lhs.getVariable() < rhs.getVariable();
}