<?php
/**
 * @file
 * Admin functions
 *
 * @ingroup rebuild_more
 * @{
 */

/**
 * Form builder. Configure settings for rebuild_more.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function rebuild_more_admin_settings($form, &$form_state) {

  $form['help'] = array(
    '#markup' => t('These adjustments to memory and time limits only apply to batch operations for rebuilding node access permissions.'),
  );

  $form['rebuild_more_batch_size'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Max nodes per batch'),
    '#description' => t('Maximum nodes in a single batch?'),
    '#default_value' => variable_get('rebuild_more_batch_size', REBUILD_MORE_BATCH_SIZE),
    '#required' => TRUE,
  );

  $form['rebuild_more_chunk_size'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Single batch pass node count'),
    '#description' => t('How many nodes are processed in one pass of one batch?'),
    '#default_value' => variable_get('rebuild_more_chunk_size', REBUILD_MORE_CHUNK_SIZE),
    '#required' => TRUE,
  );

  $form['rebuild_more_time_limit'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Time limit (in seconds)'),
    '#default_value' => variable_get('rebuild_more_time_limit', REBUILD_MORE_TIME_LIMIT),
    '#required' => TRUE,
  );

  $form['rebuild_more_memory_limit'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Memory limit (in Megabytes)'),
    '#default_value' => variable_get('rebuild_more_memory_limit', ini_get('memory_limit')),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}