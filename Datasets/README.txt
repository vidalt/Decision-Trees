==============================================
  IMPORTANT NOTES ABOUT THE DATASET FORMAT
==============================================

• All datasets come from the UCI repository: 
https://archive.ics.uci.edu/ml/index.php

• The numbering of the datasets follows the order from: 
Bertsimas, D., & Dunn, J. (2017). Optimal classification trees. 
Machine Learning, 106(7), 1039–1082

• The datasets have been formatted to be easily readable.
The first lines give some basic information on the datasets: number of samples, attributes, classes and type of attributes.
Then each sample is described by its list of attributes, and finally by its class.
Attributes can be numerical or categorical.
Categories are marked as 0,1,2 etc...
Classes are marked as 0,1,2 etc... (the numbers only represent membership but not an order relationship in this case)

==============================================
  		DATA TREATMENT
==============================================

• Missing numerical values have been replaced by the average of the values:
Applied to:
- BREAST_CANCER (15 values)
- BREAST_CANCER_PROGNOSTIC (4 values)

• Missing categorical values have been replaced by the most common value
- Did not happen until now

• Some datasets use attributes some notions such as "low", "med", "high", "vhigh".
In this case the attribute is considered as numerical rather than categorical since the data suggests an order relation.









