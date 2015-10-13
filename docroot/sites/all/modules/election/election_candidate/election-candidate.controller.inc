<?php
/**
 * @file
 * Entity controller for candidates (in the Election module).
 *
 * @class
 * ElectionCandidateController
 * @extends
 * ElectionDefaultController
 */

class ElectionCandidateController extends ElectionDefaultController {

  /**
   * Overrides EntityAPIController::create(), allowing defaults to be defined.
   */
  public function create(array $values = array()) {
    $default_values = array(
      'candidate_id' => '',
      'election_id' => '',
      'post_id' => '',
      'published' => 1,
      'cstatus' => ELECTION_CANDIDATE_HOPEFUL,
      'uid' => '',
      'summary' => '',
      'manifesto' => '',
      'photo' => '',
      'proposer' => new stdClass(),
      'seconders' => array(),
      'type' => '',
      'created' => 0,
      'changed' => 0,
    );
    // Overwrite default values with supplied values.
    $values = array_merge($default_values, $values);
    return parent::create($values);
  }

}
