<?php

namespace Drupal\maps_import\Filter\Condition\Leaf\Object;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;

/**
 * Condition on object nature.
 */
class Nature extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Object nature';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'object_nature';
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    $criteria = $this->getCriteria();
    if (empty($criteria)) {
      return !$this->negate;
    }
    $criteria = $this->getCriteria();
    
    return $this->checkNegate($entity['nature'] == reset($criteria));
  }

}
