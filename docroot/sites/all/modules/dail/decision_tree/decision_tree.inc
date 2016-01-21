<?php

/**
 * @file: decision_tree.inc
 *
 * A class that implements a basic decision tree.
 */

/**
 * Load dependencies.
 *
 * If we use the autoloader, this isn't necessary.
 */
require_once(drupal_get_path('module', 'dail') . '/dail.inc');

class DecisionTree extends dail {

  private $tree = NULL;

  function __construct($collection = 'default', $fitness_func = 'entropy') {
    parent::__construct($collection);
    $this->type .= '.DecisionTree';
    $this->fitness_func = 'decision_tree_' . $fitness_func;
  }

  /**
   * Returns a new decision tree based on the examples given.
   */
  public function create($data, $attributes, $target_attr) {
    $vals = $this->get_values($data, $target_attr);

    $default = $this->majority_value($vals);

    if (empty($data) || (count($attributes) - 1 <= 0)) {
      return $default;
    }

    // If all records in the dataset have the same classification, return the
    // classification.
    elseif (count(array_unique($vals)) == 1) {
      return $vals[0];
    }

    $best = $this->choose_attribute($data, $attributes, $target_attr);

    $tree = array($best => array());

    foreach (array_unique($this->get_values($data, $best)) as $val) {
      $subtree = $this->create_decision_tree(
        $this->get_examples($data, $best, $val),
        array_diff($attributes, array($best)),
        $target_attr,
        $fitness_func
      );
      $tree[$best][$val] = $subtree;
    }

    $this->tree = $tree;

    return $tree;
  }

  function get_values($data, $target_attr) {
    $vals = array();

    foreach ($data as $record) {
      $vals[] = $record[$target_attr];
    }

    return $vals;
  }

  /**
   * Creates a list of all values in the target attribute for each record in the
   * data list object, and returns the value that appears in this list the most
   * frequently.
   */
  function majority_value($vals) {
    $vals = array_count_values($vals);
    asort($vals);

    return key($vals);
  }

  /**
   * Cycles through all the attributes and returns the one with the highest
   * information gain (or lowest entropy).
   */
  function choose_attribute($data, $attributes, $target_attr) {
    $best_gain = 0.0;
    $best_attr = NULL;

    // Remove $target_attr from $attributes.
    $attributes = unset($attributes[array_search($target_attr, $attributes)]);

    foreach ($attributes as $attr) {
      $gain = decision_tree_gain($data, $attr, $target_attr, $this->fitness_func);
      if ($gain >= $best_gain) {
        $best_gain = $gain;
        $best_attr = $attr;
      }
    }
    return $best_attr;
  }

  /**
   * Returns a list of all the records in $data with the value of $attr matching
   * the given $value.
   */
  function get_examples($data, $attr, $value) {
    $list = array();
    foreach ($data as $record) {
      if ($record[$attr] === $value) {
        $list[] = $record;
      }
    }
    return $list;
  }

  /**
   * Recursively traverse the decision tree and return a classification for the
   * given record.
   */
  function get_classification($record, $tree) {
    if (is_string($tree)) {
      return $tree;
    }

    $attrs = array_keys($tree);
    $attr = $attrs[0];
    $t = $tree[$attr][$record[$attr]];

    return $this->get_classification($record, $t);
  }

  /**
   * Returns a list of classifications for each of the records in the data lists
   * as determined by the given decision tree.
   */
  public function classify($data) {
    $classification = array();

    foreach ($data as $record) {
      $classification[] = $this->get_classification($record, $this->tree);
    }
    return $classification;
  }
}
