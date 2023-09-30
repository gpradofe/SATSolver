#include "BooleanFormula.h"
#include "BacktrackSolver.h"
#include <iostream>
#include <chrono>
#include <fstream>

std::string getBaseFilename(const std::string &path)
{
    size_t last_slash_pos = path.find_last_of("/\\");
    size_t last_dot_pos = path.find_last_of('.');

    size_t start = (last_slash_pos == std::string::npos) ? 0 : last_slash_pos + 1;
    size_t end = (last_dot_pos == std::string::npos || last_dot_pos <= start) ? path.length() : last_dot_pos;

    return path.substr(start, end - start);
}

int main()
{
    std::string filename;
    std::cout << "Enter the path to the SAT formula file: ";
    std::cin >> filename;

    BooleanFormula loader;
    auto formulas = loader.loadFromFile(filename);
    if (formulas.empty())
    {
        std::cerr << "Failed to load formulas from the file." << std::endl;
        return 1;
    }

    std::string base_filename = getBaseFilename(filename);
    std::ofstream csv_file(base_filename + ".csv");
    csv_file << "Formula #,Elapsed Time (ms),Result" << std::endl;

    int formula_num = 1;
    for (auto &formula : formulas)
    {
        std::cout << "Solving formula #" << formula_num << std::endl;

        BacktrackSolver solver(formula);
        auto start_time = std::chrono::high_resolution_clock::now();
        bool solution_found = solver.solve();
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        csv_file << formula_num << "," << elapsed_time << ",";

        if (solution_found)
        {
            csv_file << "Satisfying assignment found" << std::endl;

            std::cout << "Satisfying assignment found!" << std::endl;
            const auto &assignment = solver.getAssignment();
            for (size_t i = 0; i < assignment.size(); ++i)
            {
                std::cout << "Variable " << (i + 1) << ": ";
                switch (assignment[i])
                {
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
        }
        else
        {
            std::cout << "No satisfying assignment found." << std::endl;
            csv_file << "No satisfying assignment found" << std::endl;
        }

        std::cout << "----------------------------" << std::endl;
        formula_num++;
    }

    csv_file.close();

    return 0;
}
