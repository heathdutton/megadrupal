<?php

/**
 * @file
 * Devel Generate batch handling functions using the BatchAPI
 */

/**
 * Batch function for creating spaces.
 */
function studyroom_devel_generate_batch_spaces($form_state) {
  $operations = array();

  // Setup the batch operations and save the variables.
  $operations[] = array('studyroom_devel_generate_batch_spaces_pre_space', array($form_state['values']));

  // Add the kill operation.
  if ($form_state['values']['kill_spaces']) {
    $operations[] = array('studyroom_devel_generate_batch_spaces_kill', array());
  }

  // Add the operations to create the spaces.
  for ($num = 0; $num < $form_state['values']['num_spaces']; $num++) {
    $operations[] = array('studyroom_devel_generate_batch_spaces_add_space', array());
  }

  // Start the batch.
  $batch = array(
    'title' => t('Generating spaces'),
    'operations' => $operations,
    'finished' => 'studyroom_devel_generate_batch_spaces_finished',
    'file' => drupal_get_path('module', 'studyroom_devel_generate') . '/devel_generate_batch.inc',
  );
  batch_set($batch);
}

/**
 * Batch Functions - delete all spaces.
 */
function studyroom_devel_generate_batch_spaces_kill(&$context) {
  module_load_include('inc', 'studyroom_devel_generate', 'devel_generate');
  studyroom_devel_generate_spaces_kill($context['results']);
}

/**
 * Batch Functions - pre space.
 */
function studyroom_devel_generate_batch_spaces_pre_space($vars, &$context) {
  $context['results'] = $vars;
  $context['results']['num_space_ids'] = 0;
  studyroom_devel_generate_pre_users($context['results']);
}

/**
 * Batch Functions - add one space.
 */
function studyroom_devel_generate_batch_spaces_add_space(&$context) {
  module_load_include('inc', 'studyroom_devel_generate', 'devel_generate');
  module_load_include('inc', 'devel_generate', 'devel_generate');
  studyroom_devel_generate_spaces_add_space($context['results']);
  $context['results']['num_space_ids']++;
}

/**
 * Batch Functions - finished message.
 */
function studyroom_devel_generate_batch_spaces_finished($success, $results, $operations) {
  if ($success) {
    $message = t('Finished @num_space_ids spaces created successfully.', array('@num_space_ids' => $results['num_space_ids']));
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

/**
 * Batch function for creating reservations.
 */
function studyroom_devel_generate_batch_reservations($form_state) {
  $operations = array();

  // Setup the batch operations and save the variables.
  $operations[] = array('studyroom_devel_generate_batch_reservations_pre_reservation', array($form_state['values']));

  // Add the kill operation.
  if ($form_state['values']['kill_reservations']) {
    $operations[] = array('studyroom_devel_generate_batch_reservations_kill', array());
  }

  // Add the operations to create the spaces.
  for ($num = 0; $num < $form_state['values']['num_reservations']; $num++) {
    $operations[] = array('studyroom_devel_generate_batch_reservations_add_reservation', array());
  }

  // Start the batch.
  $batch = array(
    'title' => t('Generating reservations'),
    'operations' => $operations,
    'finished' => 'studyroom_devel_generate_batch_reservations_finished',
    'file' => drupal_get_path('module', 'studyroom_devel_generate') . '/devel_generate_batch.inc',
  );
  batch_set($batch);
}

/**
 * Batch Functions - delete all reservations.
 */
function studyroom_devel_generate_batch_reservations_kill(&$context) {
  module_load_include('inc', 'studyroom_devel_generate', 'devel_generate');
  studyroom_devel_generate_reservations_kill($context['results']);
}

/**
 * Batch Functions - pre reservations.
 */
function studyroom_devel_generate_batch_reservations_pre_reservation($vars, &$context) {
  $context['results'] = $vars;
  $context['results']['num_reservation_ids'] = 0;
  studyroom_devel_generate_pre_users($context['results']);
}

/**
 * Batch Functions - add one reservations.
 */
function studyroom_devel_generate_batch_reservations_add_reservation(&$context) {
  module_load_include('inc', 'studyroom_devel_generate', 'devel_generate');
  module_load_include('inc', 'devel_generate', 'devel_generate');
  $status = studyroom_devel_generate_reservations_add_reservation($context['results']);
  if ($status) {
    $context['results']['num_reservation_ids']++;
  }
}

/**
 * Batch Functions - finished message.
 */
function studyroom_devel_generate_batch_reservations_finished($success, $results, $operations) {
  if ($success) {
    $message = t('Finished @num_reservation_ids reservations created successfully.',
      array('@num_reservation_ids' => $results['num_reservation_ids']));
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}
