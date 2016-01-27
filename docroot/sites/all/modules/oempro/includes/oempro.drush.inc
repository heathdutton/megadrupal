<?php

/**
 * @file
 * Oempro module Drush integration
 */

/**
 * Implements hook_drush_command().
 *
 * @return
 *   An associative array of commands.
 */
function oempro_drush_command() {
  $items = array();

  $items['oempro-cron'] = array(
    'callback' => '_oempro_cron_batch',
    'description' => "Trigger Oempro cron task",
    'examples' => array(
      'drush oempro-cron 5000' => 'Run Oempro cron with a batch size of 5000.',
      'drush oempro-cron' => 'Run a Oempro cron task.',
    ),
  );

  return $items;
}

function _oempro_cron_batch($temp_batchsize = NULL) {
  if (!is_null($temp_batchsize)) {
    $stored_batchsize = variable_get('oempro_batch_limit', 100);
    variable_set('oempro_batch_limit', $temp_batchsize);
  }
  oempro_lists_cron();
  if (!is_null($batchsize)) {
    variable_set('oempro_batch_limit', $stored_batchsize);
  }
}
