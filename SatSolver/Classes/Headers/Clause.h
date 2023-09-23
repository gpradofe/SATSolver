#pragma once
#include "Literal.h"
#include <vector>

class Clause
{
public:
    Clause() = default;                           // Default constructor
    Clause(const std::vector<Literal> &literals); // New constructor added
    void addLiteral(const Literal &literal);
    BoolValue evaluate(const std::vector<BoolValue> &assignments) const;
    const std::vector<Literal> &getLiterals() const;
    void clear();

private:
    std::vector<Literal> literals;
};
