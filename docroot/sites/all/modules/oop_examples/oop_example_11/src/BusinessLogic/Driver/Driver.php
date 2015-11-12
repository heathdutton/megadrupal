<?php
/**
 * @file
 * Driver class.
 */

namespace Drupal\oop_example_11\BusinessLogic\Driver;

use Drupal\oop_example_11\BusinessLogic\Vehicle\DriveInterface;

/**
 * Driver class.
 */
class Driver {

  /**
   * The driver's name.
   *
   * @var string
   */
  public $name;

  /**
   * Drivable object, probably Vehicle, but not necessary.
   *
   * @var DriveInterface
   */
  private $drivableObject;

  /**
   * Class constructor.
   */
  public function __construct($name, DriveInterface $drivable_object) {
    $this->name = $name;
    $this->drivableObject = $drivable_object;
  }

  /**
   * Makes driving by directions.
   */
  public function driveByDirections(array $directions) {

    $log = $this->name . ' starts driving. <br />';

    foreach ($directions as $value) {
      if (is_numeric($value)) {
        $log .= $this->drivableObject->driveStraight($value);
        $log .= '<br />';
      }
      elseif ($value == 'Left') {
        $log .= $this->drivableObject->turnLeft();
        $log .= '<br />';
      }
      elseif ($value == 'Right') {
        $log .= $this->drivableObject->turnRight();
        $log .= '<br />';
      }
    }

    $log .= $this->name . ' finishes driving. <br />';

    return $log;
  }

}
