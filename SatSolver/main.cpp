#include "BooleanFormula.h"
#include "BacktrackSolver.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include <sstream>

// Maximum threads that can be run simultaneously.
const int MAX_THREADS = 8;

/**
<summary>
Extracts the base filename from a given file path.
</summary>
<param name="path">The full path to the file.</param>
<returns>The base filename without path or extension.</returns>
*/
std::string getBaseFilename(const std::string &path)
{
    size_t last_slash_pos = path.find_last_of("/\\");
    size_t last_dot_pos = path.find_last_of('.');

    size_t start = (last_slash_pos == std::string::npos) ? 0 : last_slash_pos + 1;
    size_t end = (last_dot_pos == std::string::npos || last_dot_pos <= start) ? path.length() : last_dot_pos;

    return path.substr(start, end - start);
}

// A structure to store results from the SAT problem evaluation.
struct FormulaResult
{
    int index;
    std::string output;
    std::string csv_data;
};

/**
<summary>
Processes a SAT formula and evaluates its satisfiability.
</summary>
<param name="index">The index of the formula in the formulas vector.</param>
<param name="formulas">A reference to the vector of all formulas.</param>
<param name="results">A reference to the vector storing results.</param>
<param name="total_wffs">Total number of well-formed formulas processed.</param>
<param name="total_satisfiable">Total number of satisfiable formulas.</param>
<param name="total_unsatisfiable">Total number of unsatisfiable formulas.</param>
<param name="total_answer_provided">Total formulas for which an answer was provided.</param>
<param name="total_correct_answers">Total number of correctly answered formulas.</param>
<param name="mtx">Mutex for handling concurrent accesses.</param>
*/
void processFormula(int index, const std::vector<BooleanFormula> &formulas, std::vector<FormulaResult> &results, int &total_wffs, int &total_satisfiable, int &total_unsatisfiable, int &total_answer_provided, int &total_correct_answers, std::mutex &mtx)
{
    BooleanFormula formula = formulas[index];
    std::stringstream console_output, csv_output;

    console_output << "Solving formula #" << index + 1 << "\n";

    char provided_answer = formula.getAnswer();
    console_output << "Answer: " << provided_answer << "\n";

    console_output << "Max literals in a clause: " << formula.getMaxLiteralsInClause() << "\n";

    BacktrackSolver solver(formula);
    auto start_time = std::chrono::high_resolution_clock::now();
    bool solution_found = solver.solve();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    csv_output << index + 1 << ","
               << formula.getVariableCount() << ","
               << formula.getClauseCount() << ","
               << formula.getMaxLiteralsInClause() << ","
               << formula.getTotalLiterals() << ",";

    if (solution_found)
    {
        csv_output << "S,";
        console_output << "Satisfiable answer found for formula #" << index + 1 << "\n";
        const auto &assignment = solver.getAssignment();
        for (BoolValue val : assignment)
        {
            console_output << static_cast<int>(val) << " ";
        }
        console_output << "\n";

        total_satisfiable++;
    }
    else
    {
        csv_output << "U,";
        console_output << "No satisfiable answer found for formula #" << index + 1 << "\n";
        total_unsatisfiable++;
    }

    if (provided_answer == 'S' && solution_found)
    {
        csv_output << "1,";
        total_correct_answers++;
    }
    else if (provided_answer == 'U' && !solution_found)
    {
        csv_output << "1,";
        total_correct_answers++;
    }
    else if (provided_answer != '?')
    {
        csv_output << "-1,";
        total_correct_answers++;
    }
    else
    {
        csv_output << "0,";
    }

    csv_output << elapsed_time << ",";
    total_answer_provided += (provided_answer != '?');

    const auto &assignment = solver.getAssignment();
    for (size_t i = 0; i < assignment.size(); ++i)
    {
        switch (assignment[i])
        {
        case BoolValue::TRUE:
            csv_output << "1,";
            break;
        case BoolValue::FALSE:
            csv_output << "0,";
            break;
        default:
            csv_output << "-1,";
            break;
        }
    }
    csv_output << "\n";

    mtx.lock();
    total_wffs++;
    results[index] = {index, console_output.str(), csv_output.str()};
    mtx.unlock();

    console_output << "----------------------------\n";
}

int main()
{
    std::string filename;
    // Ask the user for the SAT file to be processed.
    std::cout << "Enter the path to the SAT formula file: ";
    std::cin >> filename;

    // Load the SAT formulas from the provided file.
    BooleanFormula loader;
    auto formulas = loader.loadFromFile(filename);
    if (formulas.empty())
    {
        std::cerr << "Failed to load formulas from the file." << std::endl;
        return 1;
    }

    // Set up the CSV file for output.
    std::string base_filename = getBaseFilename(filename);
    std::ofstream csv_file(base_filename + ".csv");
    csv_file << "Problem Number,Number of Variables,Number of Clauses,Max Literals in a Clause,Total Literals,S/U,Agreement,Execution Time in Microseconds,Assignments..." << std::endl;

    // Initialize counters and containers.
    int total_wffs = 0, total_satisfiable = 0, total_unsatisfiable = 0, total_answer_provided = 0, total_correct_answers = 0;
    std::mutex mtx;
    std::vector<std::thread> workers;
    std::vector<FormulaResult> results(formulas.size());

    // Launch threads to process formulas.
    for (size_t i = 0; i < formulas.size(); ++i)
    {
        workers.emplace_back(processFormula, i, std::ref(formulas), std::ref(results), std::ref(total_wffs), std::ref(total_satisfiable), std::ref(total_unsatisfiable), std::ref(total_answer_provided), std::ref(total_correct_answers), std::ref(mtx));

        if (workers.size() == MAX_THREADS || i == formulas.size() - 1)
        {
            for (auto &worker : workers)
            {
                worker.join();
            }
            workers.clear();
        }
    }

    // Output the results.
    for (const auto &result : results)
    {
        std::cout << result.output;
        csv_file << result.csv_data;
    }

    // Append the summary results to the CSV file.
    csv_file << "Filename,Team Name,Total WFFs,Satisfiable WFFs,Unsatisfiable WFFs,WFFs with Provided Answers,Correctly Answered WFFs" << std::endl;
    csv_file << base_filename << ",OrozcoAniceto," << total_wffs << "," << total_satisfiable << "," << total_unsatisfiable << "," << total_answer_provided << "," << total_correct_answers << std::endl;

    csv_file.close();
    return 0;
}
