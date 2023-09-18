#pragma once
#include "BoolValue.h"

class Literal {
public:
    Literal(int var, BoolValue val);

    // Evaluate literal based on assignment
    BoolValue evaluate(BoolValue assignment) const;

    // Getter methods
    int getVariable() const;
    BoolValue getValue() const;

private:
    int variable; // Variable number
    BoolValue value; // TRUE, FALSE, or UNASSIGNED
};
