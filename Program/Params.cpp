#include "Params.h"

Params::Params(std::string pathToInstance, std::string pathToSolution, int seedRNG, int maxDepth, clock_t maxTime) : seed(seedRNG), pathToInstance(pathToInstance), pathToSolution(pathToSolution), maxDepth(maxDepth), maxTime(maxTime)
{
	// Initializing random number generator here (if you have nondeterministic components)
	// std::srand(seedRNG);
	// std::cout << "----- INITIALIZING RNG WITH SEED: " << seedRNG << std::endl;

	std::ifstream inputFile(pathToInstance.c_str());
	if (inputFile.is_open())
	{
		// Reading the dataset
		std::string useless, attType;
		inputFile >> useless >> datasetName ;
		inputFile >> useless >> nbSamples;
		inputFile >> useless >> nbAttributes;
		inputFile >> useless;
		for (unsigned int i = 0; i < nbAttributes; i++)
		{
			inputFile >> attType;
			if (attType == "C") attributeTypes.push_back(TYPE_CATEGORICAL);
			else if (attType == "N") attributeTypes.push_back(TYPE_NUMERICAL);
			else throw std::string("ERROR: non recognized attribute type");
		}
		inputFile >> useless >> nbClasses;
		dataAttributes = std::vector<std::vector<double> >(nbSamples, std::vector<double>(nbAttributes));
		dataClasses    = std::vector<int>(nbSamples);
		for (unsigned int s = 0; s < nbSamples; s++)
		{
			for (unsigned int i = 0; i < nbAttributes; i++)
				inputFile >> dataAttributes[s][i];
			inputFile >> dataClasses[s];
			dataClasses[s]--; //By convention the first class with take value 0 (TODO -- could modify instances to be OK)
		}
		inputFile >> useless;
		if (!(useless == "EOF"))
			throw std::string("ERROR when reading instance, EOF has not been found where expected");
		std::cout << "----- DATASET [" << datasetName << "] LOADED IN " << clock()/ (double)CLOCKS_PER_SEC << "(s)" << std::endl;
	}
	else
		std::cout << "----- IMPOSSIBLE TO OPEN DATASET: " << pathToInstance << std::endl;
}