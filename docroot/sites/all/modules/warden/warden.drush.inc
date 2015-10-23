<?php

/**
 * @file
 * Drush integration with the Warden module.
 */

 /**
  * Implements of hook_drush_command().
  */
function warden_drush_command() {
  $items = array();

  $items['warden-check-connection'] = array(
    'description' => 'Test the connection to Warden by getting its public key',
  );

  $items['warden-update'] = array(
    'description' => 'Update Warden with the lastest site data',
  );

  return $items;
}

/**
 * Test the connection to warden.
 */
function drush_warden_check_connection() {
  drush_print(dt('Going to check connection to Warden server by retreiving the public key ...'));

  try {
    $key = warden_get_api()->getPublicKey();
    drush_print($key);
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * Test the connection to warden.
 */
function drush_warden_update() {
  module_load_include('inc', 'warden', 'warden.page');
  drush_print(dt('Going to update Warden ...'));

  try {
    _warden_update_warden();
    drush_print(dt('... success'));
  }
  catch (Exception $e) {
    return drush_set_error($e->getMessage());
  }
}
