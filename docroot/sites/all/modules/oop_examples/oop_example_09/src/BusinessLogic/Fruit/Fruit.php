<?php

/**
 * @file
 * Fruit class.
 */

namespace Drupal\oop_example_09\BusinessLogic\Fruit;

use Drupal\oop_example_09\BusinessLogic\Common\ColorInterface;

/**
 * Fruit class.
 */
class Fruit implements ColorInterface, JuiceInterface {
  /**
   * Returns color of the object.
   */
  public function getColor() {
    return t('green');
  }

  /**
   * Makes juice.
   */
  public function getJuice() {
    // Juicing code is omitted.
    return "Some juice.";
  }

}
