<?php
/**
 * @file
 * Entity controller for the 'election' entity in the Election module.
 *
 * @class
 * ElectionController
 * @extends
 * ElectionDefaultController
 */

class ElectionController extends ElectionDefaultController {

  /**
   * Overrides EntityAPIController::create(), allowing defaults to be defined.
   */
  public function create(array $values = array()) {
    $default_values = array(
      'election_id' => '',
      'type' => 0,
      'title' => '',
      'uid' => NULL,
      'published' => 0,
      'vstatus' => ELECTION_STATUS_CLOSED,
      'vopen_time' => NULL,
      'vclose_time' => NULL,
      'nstatus' => ELECTION_STATUS_CLOSED,
      'nopen_time' => NULL,
      'nclose_time' => NULL,
      'settings' => array(),
    );
    // Overwrite default values with supplied values.
    $values = array_merge($default_values, $values);
    return parent::create($values);
  }

}
