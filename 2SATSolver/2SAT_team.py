from collections import defaultdict
import csv
import time
from typing import List, Tuple, Union

class TwoSAT:
    def __init__(self, n: int):
        # Number of variables in the problem
        self.n = n
        
        # The implication graph and its reverse
        self.graph = defaultdict(list)
        self.rev_graph = defaultdict(list)

    def add_clause(self, a: int, b: int):
        """
        Add a clause to the 2-SAT problem.

        Args:
            a (int): First variable of the clause.
            b (int): Second variable of the clause.
        """
        # Create the implication edges based on the clause (a OR b)
        # If a is false then b must be true and vice versa
        self.graph[-a].append(b)
        self.graph[-b].append(a)
        
        # Add edges to the reversed graph
        self.rev_graph[b].append(-a)
        self.rev_graph[a].append(-b)

    def kosaraju_scc(self) -> List[List[int]]:
        """
        Use Kosaraju's algorithm to find Strongly Connected Components (SCCs).

        Returns:
            List[List[int]]: List of SCCs.
        """
        visited = [False] * (2 * self.n + 1)
        order = []

        # First DFS pass on original graph to compute the post-order
        def dfs_first(v):
            visited[v] = True
            for u in self.graph[v]:
                if not visited[u]:
                    dfs_first(u)
            order.append(v)

        # Second DFS pass on the reversed graph to find SCCs
        def dfs_second(v, component):
            visited[v] = True
            for u in self.rev_graph[v]:
                if not visited[u]:
                    dfs_second(u, component)
            component.append(v)

        # Execute the first DFS on every unvisited node
        for v in range(1, 2 * self.n + 1):
            if not visited[v]:
                dfs_first(v)

        visited = [False] * (2 * self.n + 1)
        sccs = []

        # Execute the second DFS in decreasing order of post-order
        for v in reversed(order):
            if not visited[v]:
                scc = []
                dfs_second(v, scc)
                sccs.append(scc)

        return sccs

    def solve(self) -> Tuple[str, List[Union[int, None]]]:
        """
        Solve the 2-SAT problem.

        Returns:
            Tuple[str, List[Union[int, None]]]: A tuple containing the result ("satisfiable" or "unsatisfiable")
            and the variable assignments.
        """
        sccs = self.kosaraju_scc()
        assignment = [None] * (2 * self.n + 1)

        # Check if a variable and its negation are in the same SCC
        for scc in sccs:
            scc_set = set(scc)
            for v in scc:
                if -v in scc_set:
                    return "unsatisfiable", None

        # Try to assign a value to each node in the SCC
        # If a node is unassigned, its negation should also be unassigned
        for scc in sccs:
            for v in scc:
                if assignment[abs(v)] is None:
                    assignment[abs(v)] = v > 0

        return "satisfiable", assignment[1:]

def read_2sat_cnf(filename: str) -> List[Tuple[int, int, str, List[Tuple[int, int]]]]:
    """
    Read 2-SAT CNF (Conjunctive Normal Form) formulas from a file.

    Args:
        filename (str): The name of the file containing 2-SAT CNF formulas.

    Returns:
        List[Tuple[int, int, str, List[Tuple[int, int]]]]: A list of tuples containing formula information,
        including the number of variables, number of clauses, provided answer, and CNF clauses.
    """
    formulas = []
    with open(filename, "r") as file:
        clauses = []
        for line in file:
            line = line.strip()
            if line.startswith("c"):
                if clauses:  # if there are previous clauses, add them to the formulas list
                    formulas.append((Num_Vars, num_clauses, answer, clauses))
                    clauses = []
                _, formula_num, min_vars, answer = line.split(',')
                formula_num = int(formula_num)
                min_vars = int(min_vars)  # though currently, you're not using min_vars
            elif line.startswith("p"):
                _, fmt, n, n_ = line.split(',')
                Num_Vars = int(n)
                num_clauses = int(n_)
            else:
                a, b, finish = map(int, line.split(','))
                clauses.append((a, b))
        if clauses:  # Add the last set of clauses
            formulas.append((Num_Vars, num_clauses, answer, clauses))
    return formulas

filename = "2SAT.cnf"
output_filename = "2SAT_results.csv"

# Additional counters for summary stats
total_wffs = 0
satisfiable_count = 0
unsatisfiable_count = 0
answers_provided_count = 0
correct_answers_count = 0

# Open the CSV file for writing results
with open(output_filename, 'w', newline='') as csvfile:
    csv_writer = csv.writer(csvfile)
    # Adjusted header for new CSV format
    csv_writer.writerow(["Problem Number", "Number of Variables", "Number of Clauses", 
                         "Max Literals in Any Clause", "Total Number of Literals", 
                         "S/U", "1/-1/0", "Execution Time (microseconds)", "Variable Assignments"])
    
    formulas = read_2sat_cnf(filename)
    formula_num = 1  # Counter for formula number

    for Num_Vars, num_clauses, provided_answer, cnf_clauses in formulas:
        total_wffs += 1
        solver = TwoSAT(Num_Vars)

        for clause in cnf_clauses:
            solver.add_clause(*clause)

        # Measure the start time
        start_time = time.time()
    
        result, assignment = solver.solve()

        # Measure the end time
        end_time = time.time()
    
        elapsed_time = (end_time - start_time) * 1e6  # Convert to microseconds
        
        max_literals_in_clause = max(len(clause) for clause in cnf_clauses)
        total_literals = sum(len(clause) for clause in cnf_clauses)

        # Compare with provided answer
        short_result = 'S' if result == "satisfiable" else 'U'
        if provided_answer == '?':
            answer_check = 0  # No answer provided
        elif provided_answer == short_result:
            answers_provided_count += 1
            correct_answers_count += 1
            answer_check = 1  # Answer provided and matches
        else:
            answers_provided_count += 1
            answer_check = -1  # Answer provided but doesn't match

        if short_result == 'S':
            satisfiable_count += 1
            print("Satisfiable")
            print("Variable assignments:", [int(val) if val is not None else -1 for val in assignment])
        else:
            unsatisfiable_count += 1
            print("Unsatisfiable")
        
        # Here, we're using placeholders since more information is required for accurate calculation
        csv_writer.writerow([formula_num, Num_Vars, num_clauses, max_literals_in_clause, 
                            total_literals, short_result, answer_check, elapsed_time] + 
                            ([int(val) if val is not None else -1 for val in assignment] if assignment else []))
        
        formula_num += 1

    # Write the summary stats
    csv_writer.writerow([filename.split('.')[0], "OrozcoAniceto", total_wffs, satisfiable_count, 
                        unsatisfiable_count, answers_provided_count, correct_answers_count])
