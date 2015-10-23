<?php

/**
 * @file
 * Function and Class definitions for Reservation units.
 */

/**
 * The class used for ReservationUnit entities.
 */
class ReservationUnit extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'reservation_unit');
  }
}

/**
 * The Controller for Reservation unit entities.
 */
class ReservationUnitController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    $values += array(
      'unit' => '',
      'name' => '',
      'description' => '',
      'module' => 'reservation',
      'status' => ENTITY_CUSTOM,
    );

    return parent::create($values);
  }
}

/**
 * UI controller.
 */
class ReservationUnitUIController extends EntityDefaultUIController {

}
