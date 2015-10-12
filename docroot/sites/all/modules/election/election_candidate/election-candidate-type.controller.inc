<?php
/**
 * @file
 * The controller for election candidate type entities.
 */

class ElectionCandidateTypeController extends EntityAPIControllerExportable {

  /**
   * Set defaults for candidate types.
   *
   * @return object
   *   A candidate type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'id' => '',
      'is_new' => TRUE,
      'data' => '',
    );
    return parent::create($values);
  }

}
