<?php

namespace Drupal\maps_import\Filter\Condition\Operator;

use Drupal\maps_import\Filter\Condition\ConditionInterface;

/**
 * OR condition.
 */
class OperatorOr extends Operator {

  /**
   * @inheritdoc
   */
  public function getType() {
    return ConditionInterface::CONDITION_OR;
  }

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'OR';
  }

}
