Usage:
  my_executable dataset_path [options]

Available options:
  -t             CPU time in seconds (defaults to 300s).
  -sol           File where to output the solution statistics (defaults to the instance file name prepended with 'sol-').
  -depth         Max depth allowed for the decision tree

Example:
"my_executable Datasets/p04.txt -depth 5 -t 600"
