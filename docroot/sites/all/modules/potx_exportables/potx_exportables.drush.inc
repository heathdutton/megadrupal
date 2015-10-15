<?php
/**
 * @file
 * This files defines the Drush command used to kickstart the batch process.
 */

/**
 * Implements hook_drush_command().
 */
function potx_exportables_drush_command() {
  $items = array();
  $items['potx-import-all'] = array(
    'description' => 'Import all po files using Drush.',
    'callback' => '_potx_exportables_import_drush_command',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'aliases' => array('pia'),
  );
  return $items;
}

/**
 * Command callback that fires up the batch process.
 */
function _potx_exportables_import_drush_command() {
  drush_print('Pot import initiating...');
  _potx_exportables_import_data();
}
