#pragma once
#include "Clause.h"
#include <vector>

class BooleanFormula {
public:
    void addClause(const Clause& clause);

    // Evaluate the entire formula based on assignments
    BoolValue evaluate(const std::vector<BoolValue>& assignments) const;

    // Load formula from a file
    bool loadFromFile(const std::string& filename);

    // Get the total number of variables in the formula
    int getVariableCount() const;

private:
    std::vector<Clause> clauses;
};