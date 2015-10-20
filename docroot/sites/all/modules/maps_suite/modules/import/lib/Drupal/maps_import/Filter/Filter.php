<?php

namespace Drupal\maps_import\Filter;

use Drupal\maps_import\Filter\Condition\ConditionInterface;
use Drupal\maps_import\Filter\Condition\Condition;
use Drupal\maps_import\Filter\Condition\Operator\Operator;

/**
 * Class for MaPS Import Filter.
 */
class Filter implements FilterInterface {

  /**
   * The filter data.
   *
   * @var array
   */
  private $data;

  /**
   * The filter position.
   *
   * @var int
   */
  private $position = 0;

  /**
   * @inheritdoc
   */
  public function __construct(array $data = array(), $raw = TRUE) {
    $this->data = $raw ? $this->buildTree($data) : $data;
  }

  /**
   * Recursive function for building a tree from an array.
   *
   * @param $elements
   * @param $parentId
   *
   * @return array
   */
  protected function buildTree(array &$elements, $parentId = 0) {
    $branch = array();

    foreach ($elements as $k => $element) {
      if ($element->getParentId() == $parentId) {
        $children = $this->buildTree($elements, $element->getId());

        if ($children) {
          $element->children = $children;
        }

        $branch[] = $element;
      }
    }

    return $branch;
  }

  /**
   * @inheritdoc
   */
  public function getFlattenConditions() {
    $flat = array();

    $iterator = new \RecursiveIteratorIterator($this, \RecursiveIteratorIterator::SELF_FIRST);
    foreach ($iterator as $k => $v) {
      $condition = clone $v;

      if (isset($condition->children)) {
        unset($condition->children);
      }

      // @todo use getter and setter for "depth".
      $condition->depth = $iterator->getDepth();

      $flat[] = $condition;
    }

    return $flat;
  }

  /**
   * @inheritdoc
   */
  public function getConditionOperators() {
    return array(
      'Filter\\Condition\\Operator\\OperatorOr',
      'Filter\\Condition\\Operator\\OperatorAnd',
    );
  }

  /**
   * @inheritdoc
   */
  public function checkConditions(array $object, array $conditions = NULL, $operator = ConditionInterface::CONDITION_AND) {
    // Whether the filter is evaluating a child condition.
    $isChild = TRUE;

    if (is_null($conditions)) {
      $conditions = $this;
      $isChild = FALSE;
    }

    foreach ($conditions as $condition) {
      if ($condition instanceof Operator) {
        if ($condition->children) {
          $return = $this->checkConditions($object, $condition->children, $condition->getType());
        }

        // Take care of operator and nested operators that do not have any leaf.
        if (!isset($return)) {
          continue;
        }
      }
      elseif ($condition instanceof Condition) {
        $return = $condition->match($object);
      }
      // Unsupported class...
      else {
        continue;
      }

      if (!isset($pass)) {
        $pass = $return;
      }
      else {
        $pass = $this->sumConditions($pass, $return, $operator);
      }
    }

    // If we didn't get any child condition, we have to return NULL so the
    // parent operator will ignore this empty tests.
    // If we are in the main condition, we have to return TRUE, since there
    // is not any defined filter.
    return isset($pass) ? $pass : ($isChild ? NULL : TRUE);
  }

  /**
   * Apply the operator to the given boolean values.
   *
   * @param $b1
   *    The first boolean.
   *
   * @param $b2
   *    The second boolean.
   *
   * @param $operator
   *    The operator to apply.
   *
   * @return boolean
   */
  protected function sumConditions($b1, $b2, $operator) {

    switch ($operator) {

      case ConditionInterface::CONDITION_AND:
        return $b1 && $b2;

      case ConditionInterface::CONDITION_OR:
        return $b1 || $b2;

      // Unknown operator
      default:
        return FALSE;
    }
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function valid() {
    return isset($this->data[$this->position]);
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function hasChildren() {
    $current = $this->current();
    return isset($current->children );
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function next() {
    $this->position++;
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function current() {
    return $this->data[$this->position];
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function getChildren() {
    $current = $this->current();
    $class = get_class($this);
    return new $class($current->children, FALSE);
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function rewind() {
    $this->position = 0;
  }

  /**
   * @inheritdoc
   *
   * @see \RecursiveIterator
   */
  public function key() {
    return $this->position;
  }

  /**
   * @inheritdoc
   */
  public function getAvailableConditions() {
    return array();
  }

}
