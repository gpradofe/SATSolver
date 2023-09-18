#pragma once
#include "Literal.h"
#include <vector>

class Clause {
public:
    void addLiteral(const Literal& literal);
    BoolValue evaluate(const std::vector<BoolValue>& assignments) const;
    const std::vector<Literal>& getLiterals() const;  // <-- New method added

private:
    std::vector<Literal> literals;
};
