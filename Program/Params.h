#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>

enum AttributeType { TYPE_NUMERICAL, TYPE_CATEGORICAL };

class Params
{
public:

	/* GENERAL PARAMETERS */
	int seed ;						// Random seed (for randomized algorithms)
	std::string pathToInstance ;	// Path to the instance
	std::string pathToSolution ;	// Path to the solution
	clock_t startTime ;				// Time when the algorithm started (to measure CPU time of your method)

	/* PARAMETERS OF THE ALGORITHM */
	int depthLimit ;				// Depth limit for the decision tree (1 = only one split etc...)
									// Your can add parameters here

	/* DATASET INFORMATION */
	std::string datasetName;								// Name of the dataset
	int nbSamples;											// Number of samples
	int nbAttributes;										// Number of attributes
	int nbClasses;											// Number of classes
	std::vector <AttributeType> attributeTypes;				// Type of the attributes (TYPE_NUMERICAL or TYPE_CATEGORICAL)
	std::vector < std::vector < double > > dataAttributes;  // Dataset (attributes)
	std::vector < int > dataClasses;						// Dataset (classes)

	/* CONSTRUCTOR */
	Params(std::string pathToInstance, std::string pathToSolution, int seedRNG);
};
#endif

