#include "Literal.h"

Literal::Literal(int var, BoolValue val) : variable(var), value(val) {}

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

int Literal::getVariable() const
{
    return variable;
}

BoolValue Literal::getValue() const
{
    return value;
}

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

bool operator<(const Literal &lhs, const Literal &rhs)
{
    if (lhs.getVariable() == rhs.getVariable())
    {
        return lhs.getValue() < rhs.getValue();
    }
    return lhs.getVariable() < rhs.getVariable();
}