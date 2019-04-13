#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <algorithm> 
#include <time.h>
#include <math.h>

#define MY_EPSILON 0.00001
enum AttributeType { TYPE_NUMERICAL, TYPE_CATEGORICAL };

class Params
{
public:

	/* GENERAL PARAMETERS */
	std::string pathToInstance ;	// Path to the instance
	std::string pathToSolution ;	// Path to the solution
	
	/* PARAMETERS OF THE ALGORITHM */
	int seed;						// Random seed (for randomized algorithms)
	clock_t maxTime;				// CPU time limit, if you want to use such a limit in your metaheuristic
	int maxDepth ;					// Depth limit for the decision tree

	/* DATASET INFORMATION */
	std::string datasetName;								// Name of the dataset
	int nbSamples;											// Number of samples
	int nbAttributes;										// Number of attributes
	int nbClasses;											// Number of classes
	std::vector <AttributeType> attributeTypes;				// Type of the attributes (TYPE_NUMERICAL or TYPE_CATEGORICAL)
	std::vector < std::vector < double > > dataAttributes;  // Dataset: attributes of each sample
	std::vector < int > dataClasses;						// Dataset: class of each sample
	std::vector < int > nbLevels;							// Dataset: number of possible levels/categories (only for categorical attributes)

	/* TO MEASURE CPU TIME */
	clock_t startTime;				// Time when the algorithm started
	clock_t endTime;				// Time when the algorithm ended 

	/* CONSTRUCTOR */
	Params(std::string pathToInstance, std::string pathToSolution, int seedRNG, int maxDepth, clock_t maxTime);
};
#endif

