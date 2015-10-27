<?php

/**
 * @file
 * Toyota Yaris class.
 */

namespace Drupal\oop_example_10\BusinessLogic\Vehicle\Car\Toyota;

/**
 * Toyota Yaris class.
 */
class ToyotaYaris extends Toyota {

  /**
   * The doors count.
   *
   * @var int
   */
  public $doors = 5;

  /**
   * Returns class type description.
   */
  public function getClassTypeDescription() {
    $s = t('Toyota Yaris');
    return $s;
  }

}
