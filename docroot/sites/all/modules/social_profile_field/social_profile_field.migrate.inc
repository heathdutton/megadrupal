<?php

/**
 * @file
 * Support for processing social profile fields in Migrate.
 */

/**
 * Implement hook_migrate_api().
 */
function social_profile_field_migrate_api() {
  return array(
    'api' => 2,
    'field_handlers' => array('MigrateSocialProfileFieldHandler'),
  );
}

class MigrateSocialProfileFieldHandler extends MigrateSimpleFieldHandler {
  public function __construct() {
    parent::__construct(array(
      'value_key' => 'url',
      'skip_empty' => TRUE,
    ));

    $this->registerTypes(array('social_profile_field_url'));
  }
}