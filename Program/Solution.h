#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include "Params.h"

// Structure representing one node of the (orthogonal) decision tree or a leaf
class Node
{

public:

	enum {NODE_NULL, NODE_LEAF, NODE_INTERNAL} nodeType;	// Node type
	Params * params;										// Access to the problem and dataset parameters
	int splitAttribute;										// Attribute to which the split is applied (filled through the greedy algorithm)
	double splitValue;											// Threshold value for the split (for numerical attributes the left branch will be <= splitValue, for categorical will be == splitValue)					
	std::vector <int> samples;								// Samples from the training set at this node
	std::vector <int> nbSamplesClass;						// Number of samples of each class at this node (for each class)
	int nbSamplesNode;										// Total number of samples in this node
	int majorityClass;										// Majority class in this node
	int maxSameClass;										// Maximum number of elements of the same class in this node
	double entropy;											// Entropy in this node
	
	void evaluate()
	{	
		entropy = 0.0;
		for (int c = 0; c < params->nbClasses; c++)
		{
			if (nbSamplesClass[c] > 0)
			{
				double frac = (double)nbSamplesClass[c]/(double)nbSamplesNode;
				entropy -= frac * log2(frac);
				if (nbSamplesClass[c] > maxSameClass)
				{ 
					maxSameClass = nbSamplesClass[c];
					majorityClass = c;
				}
			}
		}
	}

	void addSample(int i)
	{
		samples.push_back(i);
		nbSamplesClass[params->dataClasses[i]]++;
		nbSamplesNode++;
	}

	Node(Params * params):params(params)
	{
		nodeType = NODE_NULL;
		splitAttribute = -1;
		splitValue = -1.e30;
		nbSamplesClass = std::vector<int>(params->nbClasses, 0);
		nbSamplesNode = 0;
		majorityClass = -1;
		maxSameClass = 0;
		entropy = -1.e30;
	}
};

class Solution
{

private:

	// Access to the problem and dataset parameters
	Params * params;

public:

	// Vector representing the tree
	// Parent of tree[k]: tree[(k-1)/2]
	// Left child of tree[k]: tree[2*k+1]
	// Right child of tree[k]: tree[2*k+2]
	std::vector <Node> tree;

	// Prints the final solution
	void printAndExport(std::string fileName)
	{
		int nbMisclassifiedSamples = 0;
		std::cout << std::endl << "---------------------------------------- PRINTING SOLUTION ----------------------------------------" << std::endl;
		for (int d = 0; d <= params->maxDepth; d++)
		{
			// Printing one complete level of the tree
			for (int i = pow(2, d) - 1; i < pow(2, d + 1) - 1; i++)
			{
				if (tree[i].nodeType == Node::NODE_INTERNAL)
					std::cout << "(N" << i << ",A[" << tree[i].splitAttribute << "]" << (params->attributeTypes[tree[i].splitAttribute] == TYPE_NUMERICAL ? "<=" : "=") << tree[i].splitValue << ") ";
				else if (tree[i].nodeType == Node::NODE_LEAF)
				{
					int misclass = tree[i].nbSamplesNode - tree[i].nbSamplesClass[tree[i].majorityClass];
					nbMisclassifiedSamples += misclass;
					std::cout << "(L" << i << ",C" << tree[i].majorityClass << "," << tree[i].nbSamplesClass[tree[i].majorityClass] << "," << misclass << ") ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << nbMisclassifiedSamples << "/" << params->nbSamples << " MISCLASSIFIED SAMPLES" << std::endl;
		std::cout << "---------------------------------------------------------------------------------------------------" << std::endl << std::endl;

		std::ofstream myfile;
		myfile.open(fileName.data());
		if (myfile.is_open())
		{
			myfile << "TIME(s): " << (params->endTime - params->startTime) / (double)CLOCKS_PER_SEC << std::endl;
			myfile << "NB_SAMPLES: " << params->nbSamples << std::endl;
			myfile << "NB_MISCLASSIFIED: " << nbMisclassifiedSamples << std::endl;
			myfile.close();
		}
		else
			std::cout << "----- IMPOSSIBLE TO OPEN SOLUTION FILE: " << params->pathToSolution << " ----- " << std::endl;
	}

	Solution(Params * params):params(params)
	{
		// Initializing tree data structure and the nodes inside -- The size of the tree is 2^{maxDepth} - 1
		tree = std::vector <Node>(pow(2,params->maxDepth+1)-1,Node(params));

		// The tree is initially made of a single leaf (the root node)
		tree[0].nodeType = Node::NODE_LEAF;
		for (int i = 0; i < params->nbSamples; i++) 
			tree[0].addSample(i);
		tree[0].evaluate();
	};
};
#endif
