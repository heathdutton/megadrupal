<?php
/**
 * @file
 * Entity controller for posts (in the Election module).
 *
 * @class
 * ElectionPostController
 * @extends
 * DrupalDefaultEntityController
 */

class ElectionPostController extends ElectionDefaultController {

  /**
   * Overrides parent::create().
   */
  public function create(array $values = array()) {
    $default_values = array(
      'post_id' => '',
      'election_id' => '',
      'type' => '',
      'title' => '',
      'published' => 1,
      'rids' => array(),
      'vstatus_inheritance' => ELECTION_POST_STATUS_INHERIT,
      'nstatus_inheritance' => ELECTION_POST_STATUS_INHERIT,
      'vacancy_count' => 1,
      'allow_abstention' => 1,
      'settings' => array(),
    );
    // Overwrite default values with supplied values.
    $values = array_merge($default_values, $values);
    return parent::create($values);
  }

}
