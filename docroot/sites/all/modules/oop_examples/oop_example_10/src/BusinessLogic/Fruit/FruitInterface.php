<?php

/**
 * @file
 * Fruit interface.
 */

namespace Drupal\oop_example_10\BusinessLogic\Fruit;


use Drupal\oop_example_10\BusinessLogic\Common\ColorInterface;

/**
 * Fruit interface.
 */
interface FruitInterface extends ColorInterface, JuiceInterface {

}
