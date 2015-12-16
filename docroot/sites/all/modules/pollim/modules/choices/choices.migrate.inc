<?php
/**
 * @file
 * Code for choices.migrate.inc.
 */

/**
 * Implements migrate hook_migrate_api().
 */
function choices_migrate_api() {
  $api = array(
    'api' => 2,
  );

  return $api;
}

/**
 * Field handler for Poll choices fields.
 */
class MigrateChoicesFieldHandler extends MigrateSimpleFieldHandler {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct(array(
      'skip_empty' => TRUE,
    ));
    $this->registerTypes(array('choices'));
  }
}
