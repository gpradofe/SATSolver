#include "BooleanFormula.h"
#include "BacktrackSolver.h"
#include <iostream>

int main() {
    std::string filename;
    std::cout << "Enter the path to the SAT formula file: ";
    std::cin >> filename;

    BooleanFormula formula;
    if (!formula.loadFromFile(filename)) {
        std::cerr << "Failed to load the formula from the file." << std::endl;
        return 1;
    }

    BacktrackSolver solver(formula);
    bool solution_found = solver.solve();

    if (solution_found) {
        std::cout << "Satisfying assignment found!" << std::endl;
        const auto& assignment = solver.getAssignment();
        for (size_t i = 0; i < assignment.size(); ++i) {
            std::cout << "Variable " << (i + 1) << ": ";
            switch (assignment[i]) {
                case BoolValue::TRUE:
                    std::cout << "TRUE";
                    break;
                case BoolValue::FALSE:
                    std::cout << "FALSE";
                    break;
                default:
                    std::cout << "UNASSIGNED";
                    break;
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No satisfying assignment found." << std::endl;
    }

    return 0;
}
