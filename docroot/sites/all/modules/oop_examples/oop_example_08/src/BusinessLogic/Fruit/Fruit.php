<?php

/**
 * @file
 * Fruit class.
 */

namespace Drupal\oop_example_08\BusinessLogic\Fruit;

use Drupal\oop_example_08\BusinessLogic\Common\ColorInterface;

/**
 * Fruit class.
 */
class Fruit implements ColorInterface {
  /**
   * Returns color of the object.
   */
  public function getColor() {
    return t('green');
  }

}
