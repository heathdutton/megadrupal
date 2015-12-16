<?php

namespace Drupal\maps_import\Filter\Condition\Leaf\Media;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;

/**
 * Condition on object type.
 */
class Type extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Media type';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'media_type';
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
