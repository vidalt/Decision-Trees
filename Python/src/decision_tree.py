from data_handler import Instance

import enum
import numpy as np

class NodeType(enum.Enum):
    NULL = 0
    INTERNAL = 1
    LEAF = 2

class Node:

    def __init__(self, instance):
        if not isinstance(instance,  Instance):
            raise TypeError("ERROR: instance variable is not data_handler.Instance. Type: " + type(instance))

        self.instance = instance                                     # Access to the problem and dataset parameters
        self.node_type = NodeType.NULL                               # Node type 
        self.split_attribute_id = -1                                 # Attribute to which the split is applied
        self.split_attribute_value =  float("-inf")                  # Threshold value for the split
        self.samples = []                                            # Samples from the training set at this node
        self.num_samples_per_class = np.zeros(instance.num_classes)  # Number of samples of each class at this node
        self.num_samples = 0                                         # Total number of samples in this node
        self.majority_class = -1                                     # Majority class in this node
        self.num_majority_class = 0                                  # Number of elements in the majority class
        self.entropy = float("-inf")                                 # Entropy in this node

    def evaluate(self):
        frac = self.num_samples_per_class / self.num_samples
        frac = frac[self.num_samples_per_class > 0]
        parcial = frac * np.log2(frac)
        self.entropy = -parcial.sum()
        self.majority_class_id = self.num_samples_per_class.argmax()
        self.num_majority_class = self.num_samples_per_class[self.majority_class_id]

    def add_sample(self, sample_id):
        self.samples.append(sample_id)
        self.num_samples_per_class[self.instance.data_classes[sample_id]] += 1
        self.num_samples += 1        

class Solution:

    def __init__(self, instance):
        if not isinstance(instance,  Instance):
            raise TypeError("ERROR: instance variable is not data_handler.Instance. Type: " + type(instance))
        self.instance = instance # Access to the problem and dataset parameters
        self.tree = [Node(instance) for _ in range(2**(instance.max_depth+1)-1)]
        self.tree[0].node_type = NodeType.LEAF
        for i in range(instance.num_samples):
            self.tree[0].add_sample(i)
        self.tree[0].evaluate()

    def print_and_export(self, filename):
        num_misclassified_samples = 0

        # Print solution
        print("\n---------------------------------------- PRINTING SOLUTION ----------------------------------------")        
        for d in range(self.instance.max_depth + 1):
            # Printing one complete level of the tree
            level_info = ""
            for i in range(2**d - 1, 2**(d+1) - 1):
                if self.tree[i].node_type == NodeType.INTERNAL:
                    attribute_id = self.tree[i].split_attribute_id
                    attribute_value = self.tree[i].split_attribute_value
                    attribute_type = self.instance.attribute_types[attribute_id]

                    node_template = "(N%d,A[%d]%s" + ("%f" if attribute_type == 'N' else "%d") + ") "
                    node_info = node_template % (i, attribute_id,
                                                 '<=' if attribute_type == 'N' else '=',
                                                 attribute_value)
                elif self.tree[i].node_type == NodeType.LEAF:
                    num_misclassified = self.tree[i].num_samples - self.tree[i].num_majority_class
                    num_misclassified_samples += num_misclassified

                    node_template = "(L%d,C%d,%d,%d) "
                    node_info = node_template % (i, self.tree[i].majority_class_id,
                                                 self.tree[i].num_majority_class,
                                                 num_misclassified)
                else:
                    continue
                level_info += node_info
            print(level_info)
        print("%d/%d MISCLASSIFIED SAMPLES" % (num_misclassified_samples, self.instance.num_samples))
        print("---------------------------------------------------------------------------------------------------\n")
        
        # Dump result
        with open(filename, mode='w') as fp:
            fp.write("TIME(s): " + str(0) + "\n")
            fp.write("NB_SAMPLES: " + str(self.instance.num_samples) + "\n")
            fp.write("NB_MISCLASSIFIED: " + str(num_misclassified_samples) + "\n")

class Greedy:

    def __init__(self, instance, solution):
        if not isinstance(instance,  Instance):
            raise TypeError("ERROR: instance variable is not data_handler.Instance. Type: " + type(instance))
        if not isinstance(solution,  Solution):
            raise TypeError("ERROR: solution variable is not decision_tree.Solution. Type: " + type(solution))
        self.instance = instance
        self.solution = solution

    def run(self):
        self._recursive_construction(0, 0)
    
    def _recursive_construction(self, node_id, level):
        # BASE CASES -- MAXIMUM LEVEL HAS BEEN ATTAINED OR ALL SAMPLES BELONG TO THE SAME CLASS
        num_majority_class = self.solution.tree[node_id].num_majority_class
        num_samples = self.solution.tree[node_id].num_samples
        if level >= self.instance.max_depth or  num_majority_class == num_samples :
            return

        # LOOK FOR A BEST SPLIT
        all_identical = True # To detect contradictory data
        parent_entropy = self.solution.tree[node_id].entropy
        best_gain = float("-inf")
        best_split_attribute_id = -1
        best_split_threhold = float("-inf")

        for attr_id, attr_type in enumerate(self.instance.attribute_types):
            # Define some data structures                
            sample_ids = self.solution.tree[node_id].samples
            values = self.instance.data_attributes[sample_ids, attr_id]
            class_ids = self.instance.data_classes[sample_ids]

            if attr_type == 'N':
                # CASE 1) -- FIND SPLIT WITH BEST INFORMATION GAIN FOR NUMERICAL ATTRIBUTE c                

                # Store the possible levels of this attribute among the samples (will allow to "skip" samples with equal attribute value)
                unique_values = np.unique(values)
                
                # If all sample have the same level for this attribute, it's useless to look for a split
                if unique_values.size <= 1:
                    continue
                all_identical = False

                # Order of the samples according to attribute c                
                sorted_positions = np.argsort(values)
                sorted_values = values[sorted_positions]
                sorted_class_ids = class_ids[sorted_positions]
                
                # Initially all samples are on the right
                num_samples_per_class_left = np.zeros(self.instance.num_classes, dtype=np.int)
                num_samples_per_class_right = np.copy(self.solution.tree[node_id].num_samples_per_class)                
                
                # Go through all possible attribute values in increasing order
                sample_idx = 0
                for threshold in unique_values:
                    # Iterate on all samples with this unique_values and switch them to the left
                    while sample_idx < num_samples and  sorted_values[sample_idx] < threshold + 0.000001:
                        class_id = sorted_class_ids[sample_idx]
                        num_samples_per_class_left[class_id] += 1
                        num_samples_per_class_right[class_id] -= 1
                        sample_idx += 1
                    
                    # No need to consider the case in which all samples have been switched to the left
                    if sample_idx == num_samples:
                        continue
                    
                    frac_left = num_samples_per_class_left / sample_idx
                    frac_left = frac_left[num_samples_per_class_left > 0]
                    parcial_left = frac_left * np.log2(frac_left)
                    entropy_left = -parcial_left.sum()

                    frac_right = num_samples_per_class_right / (num_samples - sample_idx)
                    frac_right = frac_right[num_samples_per_class_right > 0]
                    parcial_right = frac_right * np.log2(frac_right)
                    entropy_right = -parcial_right.sum()

                    # Evaluate the information gain and store if this is the best option found until now
                    gain = parent_entropy - (sample_idx * entropy_left + (num_samples - sample_idx) * entropy_right) / num_samples
                    if gain > best_gain:
                        best_gain = gain
                        best_split_threhold = threshold
                        best_split_attribute_id = attr_id
            else:
                # CASE 2) -- FIND BEST SPLIT FOR CATEGORICAL ATTRIBUTE c
                num_level = self.instance.num_levels[attr_id]
                num_classes = self.instance.num_classes
                levels = values.astype(np.int)

                num_samples_per_level = np.bincount(levels, minlength=num_level)
                num_samples_per_class = np.bincount(class_ids, minlength=num_classes)
                num_samples_per_class_level = np.zeros((num_classes, num_level), dtype=np.int)
                np.add.at(num_samples_per_class_level, [class_ids, levels], 1)
                
                for l in range(num_level):
                    if num_samples_per_level[l] < 1 or num_samples_per_level[l] >= num_samples:
                        continue
                    all_identical = False
                    
                    frac_level = num_samples_per_class_level[:, l] / num_samples_per_level[l]
                    frac_level = frac_level[num_samples_per_class_level[:, l] > 0]
                    parcial_level = frac_level * np.log2(frac_level)
                    entropy_level = -parcial_level.sum()

                    frac_others = (num_samples_per_class - num_samples_per_class_level[:, l]) / (num_samples - num_samples_per_level[l])
                    frac_others = frac_others[(num_samples_per_class - num_samples_per_class_level[:, l]) > 0]
                    parcial_others = frac_others * np.log2(frac_others)
                    entropy_others = -parcial_others.sum()

                    gain = parent_entropy - (num_samples_per_level[l] * entropy_level + (num_samples - num_samples_per_level[l]) * entropy_others) / num_samples
                    if gain > best_gain:
                        best_gain = gain
                        best_split_threhold = l
                        best_split_attribute_id = attr_id                    

                """
                all_identical = ~np.logical_and(num_samples_per_level > 0, num_samples_per_level < num_samples).any()
                if all_identical: 
                    return

                frac_levels = num_samples_per_class_level / (num_samples_per_level)
                parcial_levels = frac_levels * np.log2(frac_levels)
                entropy_levels = -parcial_levels[num_samples_per_level > 0].sum(axis=0)

                frac_others = (num_samples_per_class - num_samples_per_class_level) / (num_samples_per_class - num_samples_per_level)
                parcial_others = frac_others * np.log2(frac_others)
                entropy_others = -parcial_others[:, num_samples_per_class - num_samples_per_level > 0].sum(axis=0)

                gains = parent_entropy - (num_samples_per_level * entropy_levels + (num_samples - num_samples_per_level) * entropy_others) / num_samples                
                gain = gains.max()
                if gain > best_gain:
                    best_gain = gain
                    best_split_threhold = gains.argmax()
                    best_split_attribute_id = attr_id
                """

        # SPECIAL CASE TO HANDLE POSSIBLE CONTADICTIONS IN THE DATA    
        # (Situations where the same samples have different classes -- In this case no improving split can be found)
        if all_identical: 
            return

        # APPLY THE SPLIT AND RECURSIVE CALL
        self.solution.tree[node_id].split_attribute_id = best_split_attribute_id
        self.solution.tree[node_id].split_attribute_value = best_split_threhold
        self.solution.tree[node_id].node_type = NodeType.INTERNAL
        self.solution.tree[2*node_id+1].node_type = NodeType.LEAF
        self.solution.tree[2*node_id+2].node_type = NodeType.LEAF
        for sample_id in self.solution.tree[node_id].samples:
            attribute_type = self.instance.attribute_types[best_split_attribute_id]
            attribute_value = self.instance.data_attributes[sample_id, best_split_attribute_id]
            if (attribute_type == 'N' and attribute_value <= best_split_threhold) or (attribute_type == 'C' and attribute_value == best_split_threhold):
                self.solution.tree[2*node_id+1].add_sample(sample_id)
            else:
                self.solution.tree[2*node_id+2].add_sample(sample_id)
        self.solution.tree[2*node_id+1].evaluate() # Setting all other data structures
        self.solution.tree[2*node_id+2].evaluate() # Setting all other data structures
        self._recursive_construction(2*node_id+1,level+1) # Recursive call
        self._recursive_construction(2*node_id+2,level+1) # Recursive call
