<?php
/**
 * @file
 *  Base migration class for Recruiter Demo.
 */

/**
 * Common features for all the migrations.
 */
abstract class RecruiterMigration extends Migration {

  public function __construct() {
    parent::__construct();
    ini_set('auto_detect_line_endings', TRUE);
  }

  public function processImport(array $options = array()) {
    parent::processImport($options = array());
    // Do not force menu rebuilding. Otherwise pathauto will try to rebuild
    // on each node_insert invocation.
    variable_set('menu_rebuild_needed', FALSE);
  }

  function addTermFieldMapping($destination_field, $source_field = NULL, $warn_on_override = TRUE) {
    $this->addFieldMapping($destination_field, $source_field, $warn_on_override)->separator(',');
  }

  /**
   * Helper function to parse from to Date fields.
   *
   * @param stdClass
   *   Object containing raw source data.
   */
  public function _parsePeriod(&$row, $field) {
    // Check for and add end date to time period.
    $period_temp = explode(' to ', $row->{$field});

    if (count($period_temp) === 2) {
      $date_data = array(
        'from' => (string) $period_temp[0],
        'to' => (string) $period_temp[1],
      );
      $row->{$field} = drupal_json_encode($date_data);
    }
  }

}
