<?php

namespace Drupal\maps_import\Filter\Condition\Leaf\Object;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;

/**
 * Condition on object type.
 */
class Type extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Object type';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'object_type';
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    if (!$this->getCriteria()) {
      return !$this->negate;
    }
    $criteria = $this->getCriteria();
    
    return $this->checkNegate($entity['type'] == reset($criteria));
  }

}
