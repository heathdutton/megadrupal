<?php
/**
 * @file Entity and Controller classes for the PArty Activity Type entities
 */

/**
 * Entity class for party activity types
 */
class PartyActivityType extends Entity {
  public $type;
  public $label;

  public function __construct($values = array()) {
    parent::__construct($values, 'party_activity_type');
  }
}

/**
 * Controller class for party activity types
 */
class PartyActivityTypeController extends EntityAPIControllerExportable {

  /**
   * Create a Party Activity type
   */
  public function create(array $values = array()) {
    $values += array(
      'id' => '',
      'is_new' => TRUE,
    );
    $activity_type = parent::create($values);
    return $activity_type;
  }
}
