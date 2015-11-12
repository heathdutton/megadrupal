<?php

/**
 * @file
 * Orange class.
 */

namespace Drupal\oop_example_12\BusinessLogic\Fruit;

/**
 * Orange class.
 */
class Orange extends Fruit {
  /**
   * Returns color of the object.
   */
  public function getColor() {
    return t('orange');
  }

}
