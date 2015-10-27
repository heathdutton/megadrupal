<?php

/**
 * @file
 * Banana class.
 */

namespace Drupal\oop_example_12\BusinessLogic\Fruit;

/**
 * Banana class.
 */
class Banana extends Fruit {
  /**
   * Returns color of the object.
   */
  public function getColor() {
    return t('yellow');
  }

}
