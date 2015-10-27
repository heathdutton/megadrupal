<?php

/**
 * @file
 * Orange class.
 */

namespace Drupal\oop_example_09\BusinessLogic\Fruit;

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

  /**
   * Makes juice.
   */
  public function getJuice() {
    // Juicing code is omitted.
    return "Orange juice.";
  }

}
