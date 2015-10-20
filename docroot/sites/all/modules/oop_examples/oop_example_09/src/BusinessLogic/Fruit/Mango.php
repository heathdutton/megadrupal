<?php

/**
 * @file
 * Banana class.
 */

namespace Drupal\oop_example_09\BusinessLogic\Fruit;

/**
 * Mango class.
 */
class Mango extends Fruit {
  /**
   * Returns color of the object.
   */
  public function getColor() {
    return t('yellow');
  }

  /**
   * Makes juice.
   */
  public function getJuice() {
    // Juicing code is omitted.
    return "Mango juice.";
  }

}
