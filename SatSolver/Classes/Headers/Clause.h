#pragma once
#include "Literal.h"
#include <vector>

class Clause
{
public:
    /**
    <summary>
    Default constructor for the Clause class.
    </summary>
    */
    Clause() = default; // Default constructor

    /**
    <summary>
    Constructor for the Clause class with a vector of literals.
    </summary>
    <param name="literals">A vector of literals to initialize the clause with.</param>
    */
    Clause(const std::vector<Literal> &literals); // New constructor added

    /**
    <summary>
    Adds a literal to the clause.
    </summary>
    <param name="literal">The literal to be added.</param>
    */
    void addLiteral(const Literal &literal);

    /**
    <summary>
    Evaluates the clause with the given variable assignments.
    </summary>
    <param name="assignments">A vector of BoolValue representing variable assignments.</param>
    <returns>The evaluation result as BoolValue (TRUE, FALSE, or UNASSIGNED).</returns>
    */
    BoolValue evaluate(const std::vector<BoolValue> &assignments) const;

    /**
    <summary>
    Gets the literals in the clause.
    </summary>
    <returns>A constant reference to the list of literals in the clause.</returns>
    */
    const std::vector<Literal> &getLiterals() const;

    /**
    <summary>
    Clears the clause, removing all literals.
    </summary>
    */
    void clear();

private:
    std::vector<Literal> literals;
};
