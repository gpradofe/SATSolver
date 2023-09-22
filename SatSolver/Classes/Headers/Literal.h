#pragma once
#include "BoolValue.h"

class Literal {
public:
    Literal(int var, BoolValue val);
    Literal() : variable(0), value(BoolValue::UNASSIGNED) {} // Default constructor
    // Evaluate literal based on assignment
    BoolValue evaluate(BoolValue assignment) const;

    // Getter methods
    int getVariable() const;
    BoolValue getValue() const;
    // Negate the literal
    Literal negate() const;
    // In Literal.h
    friend bool operator<(const Literal& lhs, const Literal& rhs);

private:
    int variable; // Variable number
    BoolValue value; // TRUE, FALSE, or UNASSIGNED
};
