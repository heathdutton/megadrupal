<?php

/**
 * @file
 * Migration support for the Taxonomy Term Status module.
 */

/**
 * Destination handler for Taxonomy Term Status module.
 */
class TermStatusMigrationHandler extends MigrateDestinationHandler {

  public function __construct() {
    $this->registerTypes(array('taxonomy_term'));
  }

  /**
   * Implements MigrateDestinationHandler::fields().
   */
  public function fields() {
    return array(
      'status' => t('The taxonomy term status (1 if published, 0 if not published).'),
    );
  }

}

/**
 * Implements hook_migrate_api().
 */
function termstatus_migrate_api() {
  $api = array(
    'api' => 2,
    'destination handlers' => array('TermStatusMigrationHandler'),
  );
  return $api;
}
