#include "BooleanFormula.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
void BooleanFormula::addClause(const Clause& clause) {
    clauses.push_back(clause);
}

BoolValue BooleanFormula::evaluate(const std::vector<BoolValue>& assignments) const {
    for (const Clause& clause : clauses) {
        if (clause.evaluate(assignments) == BoolValue::FALSE) {
            return BoolValue::FALSE;
        }
    }
    return BoolValue::TRUE;
}

bool BooleanFormula::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;  // Failed to open the file
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comment lines
        if (line[0] == 'c') {
            continue;
        }

        // Parse clauses
        if (line[0] != 'p') {
            std::istringstream iss(line);
            int literal_val;
            Clause clause;

            while (iss >> literal_val) {
                if (literal_val == 0) {
                    break;  // End of the clause
                }
                BoolValue literal_bool = (literal_val > 0) ? BoolValue::TRUE : BoolValue::FALSE;
                int variable = std::abs(literal_val);
                Literal literal(variable, literal_bool);
                clause.addLiteral(literal);
            }

            addClause(clause);
            std::cout << "Loaded clause: " << line << std::endl;
        }
    }

    file.close();
    std::cout << "Loaded formula with " << clauses.size() << " clauses." << std::endl;
    return true;  // Successfully loaded the formula
}

int BooleanFormula::getVariableCount() const {
    int max_variable = 0;
    for (const Clause& clause : clauses) {
        for (const Literal& lit : clause.getLiterals()) {
            max_variable = std::max(max_variable, lit.getVariable());
        }
    }
    return max_variable;
}
const std::vector<Clause>& BooleanFormula::getClauses() const {
    return clauses;
}
