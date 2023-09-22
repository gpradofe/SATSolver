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

Literal Literal::negate() const {
    if (value == BoolValue::TRUE) {
        return Literal(variable, BoolValue::FALSE);
    } else if (value == BoolValue::FALSE) {
        return Literal(variable, BoolValue::TRUE);
    }
    return Literal(variable, BoolValue::UNASSIGNED);
}

bool operator<(const Literal& lhs, const Literal& rhs) {
    if (lhs.getVariable() == rhs.getVariable()) {
        return lhs.getValue() < rhs.getValue();
    }
    return lhs.getVariable() < rhs.getVariable();
}