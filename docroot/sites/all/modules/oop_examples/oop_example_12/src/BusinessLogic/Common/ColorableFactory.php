<?php

/**
 * @file
 * ColorableFactory class.
 */

namespace Drupal\oop_example_12\BusinessLogic\Common;
use Drupal\oop_example_12\BusinessLogic\Fruit\Banana;
use Drupal\oop_example_12\BusinessLogic\Fruit\Fruit;
use Drupal\oop_example_12\BusinessLogic\Vehicle\Car\Toyota\ToyotaCamry;
use Drupal\oop_example_12\BusinessLogic\Vehicle\Car\Toyota\ToyotaYaris;
use Drupal\oop_example_12\BusinessLogic\Fruit\Orange;


/**
 * ColorableFactory class.
 *
 * Creates classes which support ColorInterface.
 */
class ColorableFactory {

  /**
   * Returns object which supports ColorInterface.
   */
  public function getColorable($class_name, $color_name = NULL) {

    $obj = new Fruit();

    switch ($class_name) {
      case 'Toyota Camry':
        $obj = new ToyotaCamry();
        $obj->color = $color_name;
        break;

      case 'Toyota Yaris':
        $obj = new ToyotaYaris();
        $obj->color = $color_name;
        break;

      case 'Banana':
        $obj = new Banana();
        break;

      case 'Orange':
        $obj = new Orange();
        break;

      default:
        // Do nothing.
        break;
    }

    return $obj;
  }

}
