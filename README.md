# 🚀 Readme-Team

## 📋 Table of Contents

- [Team Members](#-team-members)
- [Time Allocation](#-time-allocation)
- [Backtrack SAT Solver](#-backtrack-sat-solver)
  - [Code Management and Testing](#-code-management-and-testing)
  - [Programming Language and Libraries](#-programming-language-and-libraries)
  - [Key Data Structures](#-key-data-structures)
  - [Data Classes and Their Responsibilities](#-data-classes-and-their-responsibilities)
  - [Execution Time vs. Number of Variables](#-execution-time-vs-number-of-variables)
  - [Curve Fit for Worst Case Times](#-curve-fit-for-worst-case-times)
  - [Observations on Complexity](#-observations-on-complexity)
  - [Additional Work](#-additional-work)
- [TwoSAT Solver](#-twosat-solver)
  - [Introduction and Purpose](#-introduction-and-purpose)
  - [Key Data Structures and Classes](#-key-data-structures-and-classes)
  - [Algorithm Logic](#-algorithm-logic)
  - [Usage and Execution](#-usage-and-execution)

---

## 👥 Team Members:

- 🧑‍💻 **Gustavo Aniceto** - [GitHub Profile](https://github.com/gpradofe)
- 🧑‍💻 **Leticia Orozco** - [GitHub Profile](https://github.com/lettyorozcot)

---

## ⏳ Time Allocation:

**Total Time Spent:** 6 Hours

- **[Gustavo Aniceto]:** 3 Hours
- **[Leticia Orozco]:** 3 Hours

---

## 🧮 Backtrack SAT Solver:

### 🛠 Code Management and Testing:

We managed our code development through GitHub, employing a branch-per-feature strategy. Every significant feature or function was developed in its own branch and then merged into the main branch after thorough code review and testing. Automated tests were created and run after every major code update to ensure no regression.

For testing, each Clause has an evaluate method that checks if the clause is satisfied given a particular assignment. Similarly, the BooleanFormula class has methods to evaluate the entire formula and to remove redundant clauses.

### 🖥 Programming Language and Libraries:

- **Language:** C++
- **Libraries Used:** `<chrono>`, `<iostream>`, `<fstream>`, `<sstream>`, `<algorithm>`

### 🗃 Key Data Structures:

- **WFFs (Well-Formed Formulas):** The BooleanFormula class represents the Boolean formulas. This class internally manages the clauses associated with the formula using a vector (std::vector<Clause>). Each Clause represents a disjunctive collection of literals.
- **Assignments:** The BacktrackSolver class maintains the current assignment for the variables in the form of a vector (std::vector<BoolValue>). The vector is indexed based on the variable number, and each entry represents the value assigned to that variable.
- **Choice Point Stacks:** This specific data structure is not explicitly represented in the code you've shown. However, the method backtrack() effectively behaves like a depth-first search, trying out different assignments recursively.

### 📜 Data Classes and Their Responsibilities:

#### `BooleanFormula` Class:

- **Responsibility:** Represents the entire Boolean formula.
- **Attributes:**
  - Uses a vector (std::vector<Clause>) to manage the clauses associated with the formula.
- **Key Methods:**
  - `evaluate()`: Evaluates the entire formula to determine its truth value based on current variable assignments.
  - `removeRedundantClauses()`: Removes clauses that are no longer necessary or have become redundant.

#### `Clause` Class:

- **Responsibility:** Represents a single disjunctive collection of literals within the formula.
- **Attributes:**
  - Contains a collection of literals which can be positive (variable) or negative (negation of a variable).
- **Key Methods:**
  - `evaluate()`: Determines if the clause is satisfied given a particular variable assignment.

#### `BacktrackSolver` Class:

- **Responsibility:** Manages the logic for solving the Boolean formula using backtracking.
- **Attributes:**
  - Maintains the current assignment for the variables in the form of a vector (std::vector<BoolValue>).
- **Key Methods:**
  - `solve()`: Initiates the solving process for the Boolean formula.
  - `backtrack()`: Tries out different variable assignments recursively.

#### `BoolValue` Class/Enum:

- **Responsibility:** Represents the possible truth values a variable can have: TRUE, FALSE, or UNASSIGNED.
- **Usage:** Used by the `BacktrackSolver` class to maintain current variable assignments.

### 📊 Execution Time vs. Number of Variables:

The data highlights the time taken by our solver for various test files. Unsatisfiable WFFs are marked in red, while satisfiable ones are in green.

### 📈 Curve Fit for Worst Case Times:

After analyzing our results and factoring in the total number of literals, we derived a curve fit for the worst-case times.

### 🔍 Observations on Complexity:

The backtracking SAT solver as presented uses several heuristic methods to improve efficiency:

- **Unit Propagation:** This method identifies clauses where all but one literal are assigned false. The remaining unassigned literal is then set to make the clause true.
- **Pure Literal Elimination:** If a variable appears in the formula only in one polarity (either positive or negative), it can be assigned the value that satisfies all the clauses it appears in.
- **Variable Decision Heuristic:** The solver uses a heuristic based on the activity of variables in unsatisfied clauses to decide which variable to assign next. This can potentially guide the search towards a solution faster.

However, even with these heuristics, the worst-case time complexity of the backtracking SAT solver is exponential in the number of variables. This is because in the worst case, the solver might have to explore all possible assignments to find a solution or conclude unsatisfiability.

### 🌟 Additional Work:

- **File Loading:** Implemented a mechanism to load Boolean formulas from files.
- **Variable Decisions:** Implemented a heuristic where the unassigned variable with the highest frequency in unsatisfied clauses is chosen next.
- **Optimizations:** Incorporated techniques like unit propagation and pure literal elimination to simplify the formula and speed up the solving process.

---

## 🧮 TwoSAT Solver:

### 🌐 Introduction and Purpose:

The TwoSAT solver is an implementation that leverages Kosaraju's algorithm to identify strongly connected components in a directed graph, which is subsequently used to determine the satisfiability of boolean formulas expressed in the Conjunctive Normal Form (2-CNF).

### 🗃 Key Data Structures and Classes:

```python
from collections import defaultdict, deque
class TwoSAT:
    ...
```

---

### 📖 Algorithm Logic:

The solver constructs an implication graph and its reverse based on the clauses provided. Kosaraju's algorithm is applied to find strongly connected components, and the solver checks for conflicts in assignments to determine satisfiability.

### 🚀 Usage and Execution:

The provided Python script reads the CNF from a file named "2SAT.cnf". The CNF is processed, and the solver is invoked to determine satisfiability. The resulting assignments (or unsatisfiability status) are then printed to the console.

[Return to Table of Contents](#-table-of-contents)
