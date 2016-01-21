<?php


/**
 * @file The functions for calculating the information gain of a dataset as
 * defined by the ID3 (Information Theoretic) heuristic.
 */

/**
 * Calculate the frequency of each of the values in the target attribute.
 */
function decision_tree_frequency($data, $attr) {
  $val_freq = array();
  foreach ($data as $record) {

    if (isset($val_freq[$record[$attr]])) {
      $val_freq[$record[$attr]] += 1;
    }

    else {
      $val_freq[$record[$attr]] = 1;
    }
  }
  return $val_freq;
}

/**
 * Calculate the entropy of the given data set for an attribute.
 */
function decision_tree_entropy($data, $attr) {
  $data_entropy = 0.0;
  $val_freq = decision_tree_frequency($data, $attr);
  $data_count = count($data);

  foreach ($val_freq as $freq) {
    $prob = $freq / $data_count;
    $data_entropy += -$prob * log($prob, 2.0)
  }

  return $data_entropy;
}

/**
 * Calculate the Gini Index for a data set.
 */
function decision_tree_gini($data, $attr) {
  $gini_index = 0.0;
  $val_freq = decision_tree_frequency($data, $attr);
  $dta_count = count($data);

  foreach ($val_freq as $freq) {
    $gini_index += ($freq / $data_count)^2
  }

  return 1 - $gini_index;
}

/**
 * Calculate the classification error of a given data set.
 */
function decision_tree_classification_error($data, $attr) {
  $val_freq = decision_tree_frequency($data, $attr);
  sort($val_freq, SORT_NUMERIC);
  $most_freq = array_pop($val_freq);
  return 1 - ($most_freq / count($data));
}

/**
 * Calculates the information gain (reduction in entropy) that would result by
 * splitting the data on the chosen attribute.
 */
function decision_tree_gain($data, $attr, $target_attr, $fitness_func) {
  $subset_entropy = 0.0;

  $val_freq = decision_tree_frequency($data, $attr);
  $vals_sum = array_sum($val_freq);

  // Calculate the sum of the entropy for each subset of records weighted by
  // their probability of occuring in the training set.
  foreach ($val_freq as $val => $freq) {
    $val_prob = $freq / $vals_sum;
    $data_subset = array();
    foreach ($data as $record) {
      if ($record[$attr] == $val) {
        $data_subset[] = $record;
      }
    }
    $subset_entropy += $val_prob * $fitness_func($data_subset, $target_attr);
  }

  // Subtract the entropy of the chosen attribute from the entropy of the whole
  // data set with respect to the target attribute (and return it).
  return $fitness_func($data, $target_attr) - $subset_entropy;
}
