<?php

/**
 * @file
 * Migrate module integration for the profile2_privacy module.
 */

/**
 * Implements hook_migrate_api().
 */
function profile2_privacy_migrate_api() {
  $api = array(
    'api' => 2,
    'field handlers' => array(
      'MigrateProfile2PrivacyFieldHandler',
    ),
  );

  return $api;
}

/**
 * Field hander for profile2_privacy fields.
 *
 * The value passed to this field must be the 'pplid'.
 */
class MigrateProfile2PrivacyFieldHandler extends MigrateSimpleFieldHandler {
  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct(array('value_key' => 'pplid'));
    $this->registerTypes(array('profile2_privacy'));
  }
}
