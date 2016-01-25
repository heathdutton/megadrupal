<?php

namespace Drupal\maps_import\Filter\Condition\Leaf\Object;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;

/**
 * Condition on attribute class.
 *
 * @todo implement this.
 */
class Attribute extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Attribute';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'attribute';
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    return FALSE;
  }

}
