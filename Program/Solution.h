#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include "Params.h"

// Structure representing one node of the (orthogonal) decision tree or a leaf
struct Node
{
	// TYPE OF NODE:
	// LEAF represents a leaf of the decision tree
	// SPLIT_LOW represents a split / internal node for numerical   data: samples being <= than the threshold go to the left branch
	// SPLIT_EQ  represents a split / internal node for categorical data: samples being  ==  to the threshold go to the left branch
	enum {LEAF, NODE_SPLIT_LOW, NODE_SPLIT_EQ} type;	

	// For internal nodes: Index of the attribute to which the split is applied
	// For leaves: Index of the majority class (predicted values at the bottom of the tree)
	int index;								
	
	double threshold;  // Split value (undefined for leaves).
};

class Solution
{

private:

	Params * params;  // Access to the problem and dataset parameters

public:

	// Vector representing the tree
	// Node 0 corresponds to the root -- layer 0
	// Nodes 1 and 2 are the children of the root -- layer 1 etc...
	std::vector <Node> tree;

	int nbNodes;			// Metrics related to the decision tree: number of nodes
	int nbLevels;			// Metrics related to the decision tree: number of levels (constrained to a maximum value in the solver)
	int nbMisclassified;	// Metrics related to the decision tree: number of misclassified samples

	Solution(Params * params) :params(params) 
	{
		// Initializing data structure -- The size of the tree should be 2^{depthLimit} - 1
		tree = std::vector <Node>(pow(2,params->depthLimit)-1);
	};
};
#endif
