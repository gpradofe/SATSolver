from collections import defaultdict, deque
import csv
import time
class TwoSAT:
    def _init_(self, n):
        # Number of variables in the problem
        self.n = n
        
        # The implication graph and its reverse
        self.graph = defaultdict(list)
        self.rev_graph = defaultdict(list)

    def add_clause(self, a, b):
        # Create the implication edges based on the clause (a OR b)
        # If a is false then b must be true and vice versa
        self.graph[-a].append(b)
        self.graph[-b].append(a)
        
        # Add edges to the reversed graph
        self.rev_graph[b].append(-a)
        self.rev_graph[a].append(-b)

    def kosaraju_scc(self):
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

    def solve(self):
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
    
def read_2sat_cnf(filename):
    formulas = []
    with open(filename, "r") as file:
        clauses = []
        for line in file:
            if line.startswith("c"):
                continue  # Ignore comments
            elif line.startswith("p"):
                if clauses:  # if there are previous clauses, add them to the formulas list
                    formulas.append((Num_Vars, clauses))
                    clauses = []
                c, cl, n, n_ = line.split(',')
                Num_Vars = int(n)
            else:
                a, b, finish = map(int, line.split(','))
                clauses.append((a, b))
        if clauses:  # Add the last set of clauses
            formulas.append((Num_Vars, clauses))
    return formulas

filename = "2SAT.cnf"
output_filename = "2SAT_results.csv"

# Open the CSV file for writing results
with open(output_filename, 'w', newline='') as csvfile:
    csv_writer = csv.writer(csvfile)
    csv_writer.writerow(["Formula #", "Elapsed Time (ms)", "Result", "Number of Variables"])  # Write the CSV header

    formulas = read_2sat_cnf(filename)
    formula_num = 1  # Counter for formula number

    for Num_Vars, cnf_clauses in formulas:
        solver = TwoSAT(Num_Vars)

        for clause in cnf_clauses:
            solver.add_clause(*clause)

        # Measure the start time
        start_time = time.time()
    
        result, assignment = solver.solve()

        # Measure the end time
        end_time = time.time()
    
        elapsed_time = (end_time - start_time) * 1000  # Convert to milliseconds

        # Write results to CSV
        csv_writer.writerow([formula_num, elapsed_time, result, Num_Vars])
    
        # Increment the formula counter
        formula_num += 1
        if result == "satisfiable":
                print("Satisfiable")
                print("Variable assignments:", assignment)
        else:
                print("Unsatisfiable")