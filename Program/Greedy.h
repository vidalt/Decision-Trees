#ifndef GREEDY_H
#define GREEDY_H

#include "Params.h"
#include "Solution.h"

class Greedy
{
private:

	Params * params;		 // Access to the problem and dataset parameters
	Solution * solution;	 // Access to the solution structure to be filled

public:

    // Run the algorithm
    void run();

	// Constructor
	Greedy(Params * params, Solution * solution): params(params), solution(solution){};
};

#endif
