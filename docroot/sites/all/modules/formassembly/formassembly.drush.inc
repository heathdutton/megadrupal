<?php
/**
 * @file
 * Drush command file
 */




/**
 * Callback function for drush command to sync with FormAssembly
 */
function drush_formassembly_fasync() {
  $use_admin = variable_get('formassembly_admin_index', FALSE);
  $batch = array(
    'operations' => array(
      array('formassembly_batch_get_forms', array($use_admin)),
      array('formassembly_batch_extract_forms', array()),
    ),
    'finished' => 'formassembly_batch_get_finished',
    'title' => 'Request Forms Data from FormAssembly',
    'init_message' => 'Contacting FormAssembly',
  );
  batch_set($batch);
  //the current batch is not generally manipulated outside of the batch api but we need to set progressive to false
  $batch =& batch_get();
  $batch['progressive'] = false;
  drush_backend_batch_process();
}

/**
 * Implements hook_drush_help().
 */
function formassembly_drush_help($command) {
  switch ($command) {
    case 'drush:fa-sync':
      return dt('Syncronize FormAssembly Forms');
  }
}

/**
 * Implements hook_drush_command().
 */
function formassembly_drush_command() {
  $items = array();
  $items['fa-sync'] = array(
    'command-hook' => 'fasync',
    'description' => dt('Request form data from FormAssembly and sync with site database.'),
  );
  return $items;
}