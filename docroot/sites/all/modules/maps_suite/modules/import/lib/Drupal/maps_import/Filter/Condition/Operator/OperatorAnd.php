<?php

namespace Drupal\maps_import\Filter\Condition\Operator;

use Drupal\maps_import\Filter\Condition\ConditionInterface;

/**
 * AND condition.
 */
class OperatorAnd extends Operator {

  /**
   * @inheritdoc
   */
  public function getType() {
    return ConditionInterface::CONDITION_AND;
  }

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'AND';
  }

}
