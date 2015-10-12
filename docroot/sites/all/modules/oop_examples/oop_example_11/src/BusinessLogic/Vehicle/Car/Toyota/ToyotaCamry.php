<?php

/**
 * @file
 * Toyota Camry class.
 */

namespace Drupal\oop_example_11\BusinessLogic\Vehicle\Car\Toyota;

/**
 * Toyota Camry class.
 */
class ToyotaCamry extends Toyota {

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('Toyota Camry');
    return $s;
  }

}
