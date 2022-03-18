
[![C/C++ CI](https://github.com/vidalt/Decision-Trees/actions/workflows/test-cpp.yml/badge.svg)](https://github.com/vidalt/Decision-Trees/actions/workflows/test-cpp.yml)

# Purpose

This is a simplistic greedy decision tree implementation (similar to CART, but using pre-pruning via a fixed height limit rather than post-pruning).
The purpose of this code is to serve as a starting point for the first project of INF2980.
It handles continuous or categorical attributes, as well as possible contradictory data (samples with identical attribute values having different classes).
All splits are binary and based on the information gain metric:
* for continuous attributes, the algorithm chooses the best (orthogonal) split among all attributes and possible threshold values
* for categorical attributes, the algorithm chooses the best "one-versus-all" split

# Running

```
Usage:
   my_executable dataset_path [options]
Available options:
  -t             CPU time in seconds (defaults to 300s).
  -sol           File where to output the solution statistics (defaults to the instance file name prepended with 'sol-').
  -depth         Max depth allowed for the decision tree (recommended value <= 20 as the code uses a complete tree representation in the current state)
```

Example: `./my_executable Datasets/p04.txt -depth 3 -t 600`


 For your convenience, a script file for executing all experiments was created.
 
 ``` 
 sh execute.sh
```
