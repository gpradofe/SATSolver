#include "Literal.h"

Literal::Literal(int var, BoolValue val) : variable(var), value(val) {}

BoolValue Literal::evaluate(BoolValue assignment) const {
    if (value == BoolValue::UNASSIGNED) return BoolValue::UNASSIGNED;
    return (value == assignment) ? BoolValue::TRUE : BoolValue::FALSE;
}

int Literal::getVariable() const {
    return variable;
}

BoolValue Literal::getValue() const {
    return value;
}
