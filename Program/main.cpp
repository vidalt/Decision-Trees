#include "Commandline.h"
#include "Params.h"
#include "Solution.h"
#include "Greedy.h"

int main(int argc, char *argv[])
{
	Commandline c(argc, argv);
	if (c.is_valid())
	{
		// Maximum time allowed for the program
		clock_t nb_ticks_allowed = c.get_cpu_time() * CLOCKS_PER_SEC;

		// Initialization of the problem data from the commandline
		Params params(c.get_path_to_instance(), c.get_path_to_solution(), c.get_seed());

		// Initialization of a solution structure
		Solution solution(&params);

		// Optimizing the decision tree
		params.startTime = clock();
		std::cout << "----- STARTING DECISION TREE OPTIMIZATION  -----" << std::endl;
		Greedy solver(&params,&solution);
		solver.run();
		std::cout << "----- DECISION TREE OPTIMIZATION COMPLETED  -----" << std::endl;
		std::cout << "----- TIME ELAPSED: " << clock() << "   -----" << std::endl;

		// Exporting the best solution
		// TODO
		std::cout << "----- END OF ALGORITHM -----" << std::endl;
	}
	return 0;
}
