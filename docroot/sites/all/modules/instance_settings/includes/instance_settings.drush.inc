<?php

/**
 * @file
 * Executes the instance settings capabilities
 */

/**
 * Implements hook_drush_command().
 */
function instance_settings_drush_command() {
  $items = array();

  $items['instance-settings-apply'] = array(
    'description' => 'Apply instance settings to default state.',
    'drupal dependencies' => array('instance_settings'),
    'aliases' => array('isa'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function instance_settings_drush_help($section) {
  switch ($section) {
    case 'drush:instance-settings-apply':
      return dt("Applys instance settings to default state.");
  }
}

/**
 * Applies settings.
 */
function drush_instance_settings_apply() {
  drush_print(dt("Apply instance settings"));
  if (!drush_get_context('DRUSH_SIMULATE')) {
    $result = instance_settings_apply();
  }
  else {
    $result = TRUE;
  }

  if ($result) {
    drupal_set_message('Done.', 'success');
  }
}
