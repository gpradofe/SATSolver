#pragma once
#include "BoolValue.h"

class Literal
{
public:
    /**
    <summary>
    Constructor for the Literal class.
    </summary>
    <param name="var">The variable number for the literal.</param>
    <param name="val">The BoolValue representing the value of the literal (TRUE, FALSE, or UNASSIGNED).</param>
    */
    Literal(int var, BoolValue val);

    /**
    <summary>
    Default constructor for the Literal class.
    </summary>
    */
    Literal() : variable(0), value(BoolValue::UNASSIGNED) {} // Default constructor

    /**
    <summary>
    Evaluates the literal based on a given assignment.
    </summary>
    <param name="assignment">The BoolValue representing the assignment of the literal's variable.</param>
    <returns>The evaluation result as BoolValue (TRUE, FALSE, or UNASSIGNED).</returns>
    */
    BoolValue evaluate(BoolValue assignment) const;

    /**
    <summary>
    Gets the variable number of the literal.
    </summary>
    <returns>The variable number of the literal.</returns>
    */
    int getVariable() const;

    /**
    <summary>
    Gets the value of the literal (TRUE, FALSE, or UNASSIGNED).
    </summary>
    <returns>The value of the literal as BoolValue.</returns>
    */
    BoolValue getValue() const;

    /**
    <summary>
    Negates the literal, changing TRUE to FALSE and vice versa.
    </summary>
    <returns>The negated literal.</returns>
    */
    Literal negate() const;

    /**
    <summary>
    Overloads the less-than operator to enable comparisons of literals.
    </summary>
    <param name="lhs">The left-hand side literal.</param>
    <param name="rhs">The right-hand side literal.</param>
    <returns>True if lhs is less than rhs, otherwise false.</returns>
    */
    friend bool operator<(const Literal &lhs, const Literal &rhs);

private:
    int variable;    // Variable number
    BoolValue value; // TRUE, FALSE, or UNASSIGNED
};
