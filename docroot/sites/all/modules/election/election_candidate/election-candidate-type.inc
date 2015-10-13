<?php
/**
 * @file
 * The ElectionCandidateType class.
 */

/**
 * Class used for election candidate type entities.
 */
class ElectionCandidateType extends Entity {

  public $type;
  public $label;
  public $status;

  /**
   * Overrides Entity::__construct to provide the entity type.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'election_candidate_type');
  }

  /**
   * Determine whether this type is locked.
   */
  public function isLocked() {
    if (!empty($this->is_new)) {
      return FALSE;
    }
    return (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

}
