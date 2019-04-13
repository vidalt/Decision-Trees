#include "Greedy.h"

void Greedy::run()
{
	// Call the recursive procedure on the root node at level 0
	recursiveConstruction(0,0); 
}

void Greedy::recursiveConstruction(int node, int level)
{
	/* BASE CASES -- MAXIMUM LEVEL HAS BEEN ATTAINED OR ALL SAMPLES BELONG TO THE SAME CLASS */
	if (level >= params->maxDepth || solution->tree[node].maxSameClass == solution->tree[node].nbSamplesNode)
		return;

	/* LOOK FOR A BEST SPLIT */
	bool allIdentical = true; // To detect contradictory data
	int nbSamplesNode = solution->tree[node].nbSamplesNode;
	double originalEntropy = solution->tree[node].entropy;
	double bestInformationGain = -1.e30;
	int bestSplitAttribute = -1;
	double bestSplitThrehold = -1.e30;
	for (int att = 0; att < params->nbAttributes; att++)
	{
		if (params->attributeTypes[att] == TYPE_NUMERICAL)
		{
			/* CASE 1) -- FIND SPLIT WITH BEST INFORMATION GAIN FOR NUMERICAL ATTRIBUTE c */
			 
			// Define some data structures
			std::vector <std::pair<double, int>> orderedSamples;		// Order of the samples according to attribute c
			std::set<double> attributeLevels;							// Store the possible levels of this attribute among the samples (will allow to "skip" samples with equal attribute value)
			for (int s : solution->tree[node].samples)
			{
				orderedSamples.push_back(std::pair<double, int>(params->dataAttributes[s][att], params->dataClasses[s]));
				attributeLevels.insert(params->dataAttributes[s][att]);
			}
			if (attributeLevels.size() <= 1) continue;					// If all sample have the same level for this attribute, it's useless to look for a split
			else allIdentical = false;
			std::sort(orderedSamples.begin(), orderedSamples.end());
			
			// Initially all samples are on the right
			std::vector <int> nbSamplesClassLeft = std::vector<int>(params->nbClasses, 0);
			std::vector <int> nbSamplesClassRight = solution->tree[node].nbSamplesClass;
			int indexSample = 0;
			for (double attributeValue : attributeLevels) // Go through all possible attribute values in increasing order
			{
				// Iterate on all samples with this attributeValue and switch them to the left
				while (indexSample < nbSamplesNode && orderedSamples[indexSample].first < attributeValue + MY_EPSILON)
				{
					nbSamplesClassLeft[orderedSamples[indexSample].second]++;
					nbSamplesClassRight[orderedSamples[indexSample].second]--;
					indexSample++;
				}
				
				if (indexSample != nbSamplesNode) // No need to consider the case in which all samples have been switched to the left
				{
					// Evaluate entropy of the two resulting sample sets
					double entropyLeft = 0.0;
					double entropyRight = 0.0;
					for (int c = 0; c < params->nbClasses; c++)
					{
						// Remark that indexSample contains at this stage the number of samples in the left
						if (nbSamplesClassLeft[c] > 0)
						{
							double fracLeft = (double)nbSamplesClassLeft[c] / (double)(indexSample);
							entropyLeft -= fracLeft * log2(fracLeft);
						}
						if (nbSamplesClassRight[c] > 0)
						{
							double fracRight = (double)nbSamplesClassRight[c] / (double)(nbSamplesNode - indexSample);
							entropyRight -= fracRight * log2(fracRight);
						}
					}

					// Evaluate the information gain and store if this is the best option found until now
					double informationGain = originalEntropy - ((double)indexSample*entropyLeft + (double)(nbSamplesNode - indexSample)*entropyRight) / (double)nbSamplesNode;
					if (informationGain > bestInformationGain)
					{
						bestInformationGain = informationGain;
						bestSplitAttribute = att;
						bestSplitThrehold = attributeValue;
					}
				}
			}
		}
		else 
		{
			/* CASE 2) -- FIND BEST SPLIT FOR CATEGORICAL ATTRIBUTE c */

			// Count for each level of attribute c and each class the number of samples
			std::vector <int> nbSamplesLevel = std::vector <int>(params->nbLevels[att],0);
			std::vector <int> nbSamplesClass = std::vector <int>(params->nbClasses, 0);
			std::vector < std::vector <int> > nbSamplesLevelClass = std::vector< std::vector <int> >(params->nbLevels[att], std::vector <int>(params->nbClasses,0));
			for (int s : solution->tree[node].samples)
			{
				nbSamplesLevel[params->dataAttributes[s][att]]++;
				nbSamplesClass[params->dataClasses[s]]++;
				nbSamplesLevelClass[params->dataAttributes[s][att]][params->dataClasses[s]]++;
			}

			// Calculate information gain for a split at each possible level of attribute c
			for (int level = 0; level < params->nbLevels[att]; level++)
			{
				if (nbSamplesLevel[level] > 0 && nbSamplesLevel[level] < nbSamplesNode)
				{
					// Evaluate entropy of the two resulting sample sets
					allIdentical = false;
					double entropyLevel = 0.0;
					double entropyOthers = 0.0;
					for (int c = 0; c < params->nbClasses; c++)
					{
						if (nbSamplesLevelClass[level][c] > 0)
						{
							double fracLevel = (double)nbSamplesLevelClass[level][c] / (double)nbSamplesLevel[level] ;
							entropyLevel -= fracLevel * log2(fracLevel);
						}
						if (nbSamplesClass[c] - nbSamplesLevelClass[level][c] > 0)
						{
							double fracOthers = (double)(nbSamplesClass[c] - nbSamplesLevelClass[level][c]) / (double)(nbSamplesNode - nbSamplesLevel[level]);
							entropyOthers -= fracOthers * log2(fracOthers);
						}
					}

					// Evaluate the information gain and store if this is the best option found until now
					double informationGain = originalEntropy - ((double)nbSamplesLevel[level] *entropyLevel + (double)(nbSamplesNode - nbSamplesLevel[level])*entropyOthers) / (double)nbSamplesNode;
					if (informationGain > bestInformationGain)
					{
						bestInformationGain = informationGain;
						bestSplitAttribute = att;
						bestSplitThrehold = level;
					}
				}
			}
		}
	}

	/* SPECIAL CASE TO HANDLE POSSIBLE CONTADICTIONS IN THE DATA */
	// (Situations where the same samples have different classes -- In this case no improving split can be found)
	if (allIdentical) return;

	/* APPLY THE SPLIT AND RECURSIVE CALL */
	solution->tree[node].splitAttribute = bestSplitAttribute;
	solution->tree[node].splitValue = bestSplitThrehold;
	solution->tree[node].nodeType = Node::NODE_INTERNAL;
	solution->tree[2*node+1].nodeType = Node::NODE_LEAF ;
	solution->tree[2*node+2].nodeType = Node::NODE_LEAF ;
	for (int s : solution->tree[node].samples)
	{ 
		if ((params->attributeTypes[bestSplitAttribute] == TYPE_NUMERICAL   && params->dataAttributes[s][bestSplitAttribute] < bestSplitThrehold + MY_EPSILON)|| 
			(params->attributeTypes[bestSplitAttribute] == TYPE_CATEGORICAL && params->dataAttributes[s][bestSplitAttribute] < bestSplitThrehold + MY_EPSILON && params->dataAttributes[s][bestSplitAttribute] > bestSplitThrehold - MY_EPSILON))
			solution->tree[2*node+1].addSample(s);
		else
			solution->tree[2*node+2].addSample(s);
	}
	solution->tree[2*node+1].evaluate(); // Setting all other data structures
	solution->tree[2*node+2].evaluate(); // Setting all other data structures
	recursiveConstruction(2*node+1,level+1); // Recursive call
	recursiveConstruction(2*node+2,level+1); // Recursive call
}

