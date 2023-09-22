#include "BooleanFormula.h"
#include "BacktrackSolver.h"
#include <iostream>

int main() {
    std::string filename;
    std::cout << "Enter the path to the SAT formula file: ";
    std::cin >> filename;

    BooleanFormula loader;
    auto formulas = loader.loadFromFile(filename);
    if (formulas.empty()) {
        std::cerr << "Failed to load formulas from the file." << std::endl;
        return 1;
    }

    int formula_num = 1;
    for (auto& formula : formulas) {
        std::cout << "Solving formula #" << formula_num++ << std::endl;

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

        std::cout << "----------------------------" << std::endl;
    }

    return 0;
}
