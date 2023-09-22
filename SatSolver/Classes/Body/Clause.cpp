#include "Clause.h"

void Clause::addLiteral(const Literal& literal) {
    literals.push_back(literal);
}

BoolValue Clause::evaluate(const std::vector<BoolValue>& assignments) const {
    for (const Literal& lit : literals) {
        if (lit.evaluate(assignments[lit.getVariable()]) == BoolValue::TRUE) {
            return BoolValue::TRUE;
        }
    }
    
    return BoolValue::FALSE;
}

const std::vector<Literal>& Clause::getLiterals() const {
    return literals;
}
void Clause::clear() {
    literals.clear();
}

Clause::Clause(const std::vector<Literal>& literals) : literals(literals) {}
