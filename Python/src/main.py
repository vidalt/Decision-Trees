from data_handler import CommandLine, Instance
from decision_tree import Solution, Greedy

import time
import sys

if __name__ == "__main__":    
    cmd = CommandLine(sys.argv)
    
    # Initialization of the problem data from the commandline
    instance = Instance(cmd.instance_path, cmd.solution_path, cmd.seed, cmd.max_depth, cmd.cpu_time)

    # Initialization of a solution structure
    solution = Solution(instance)

    # Run the greedy algorithm
    print("----- STARTING DECISION TREE OPTIMIZATION")
    start = time.time()
    solver = Greedy(instance, solution)
    solver.run()
    delta = time.time() - start
    print("----- DECISION TREE OPTIMIZATION COMPLETED IN " + str(delta) + " (s)")
    
    # Printing the solution and exporting statistics (also export results into a file)
    solution.print_and_export(cmd.solution_path)
    print("----- END OF ALGORITHM")

    
