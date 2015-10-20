<?php

/**
 * @file
 * Contains the filter related interfaces and classes
 */

namespace Drupal\maps_import\Filter;

/**
 * Interface for MaPS Import Filter.
 */
interface FilterInterface extends \RecursiveIterator {

  /**
   * Max depth for the condition tree.
   */
  const CONDITION_MAX_DEPTH = 4;

  /**
   * The class constructor.
   *
   * @param $data
   *    The array of filter conditions.
   * @param $raw
   *    Whether the data parameter contains raw data or a fully built tree.
   */
  public function __construct(array $data = array(), $raw = TRUE);

  /**
   * Return available conditions.
   *
   * @return array
   */
  public function getAvailableConditions();

  /**
   * Return available conditions operators.
   *
   * @return array
   */
  public function getConditionOperators();

  /**
   * Read the condition tree and return conditions in a flat array.
   *
   * @return array
   */
  public function getFlattenConditions();

  /**
   * Recursive method for checking nested conditions.
   *
   * @param $object
   *    The object to test.
   * @param $conditions
   *    The condition tree to match.
   * @param $operator
   *    The current operator to apply.
   *
   * @return boolean.
   */
  public function checkConditions(array $object, array $conditions = NULL, $operator = ConditionInterface::CONDITION_AND);

}
