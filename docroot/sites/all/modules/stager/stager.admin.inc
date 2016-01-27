<?php

/**
 * @file
 * Administration functions for the stager module.
 */

/**
 * The page that shows the state of stager presenting the right info.
 */
function stager_state() {
  $build = array();

  // In staging mode.
  if (stager_staging_access()) {
    $build['stager_info'] = stager_info();
  }

  else {
    // Show list or create form.
    $build['overview'] = stager_list();
    // Only one stager site is supported.
    $created = db_query('SELECT prefix FROM {stager} WHERE prefix = :prefix', array(':prefix' => 'stager'))->fetchField();
    if (!$created) {
      $build['create_form'] = drupal_get_form('stager_create_staging');
    }
  }

  return $build;
}

/**
 * Show a list of available stager sites.
 */
function stager_list() {
  $output = '';

  $records = array();
  $results = db_select('stager', 's')
               ->fields('s', array('prefix', 'title', 'created'))
               ->orderBy('title', 'ASC')
               ->execute();

  foreach ($results as $row) {
    $record = array();
    $record[] = check_plain($row->title);
    $record[] = format_date($row->created);
    $operations = '';
    $operations .= l('Go into staging mode', 'stager/mode/staging/' . $row->prefix);
    $operations .= ' - ' . l('Sync to live', 'admin/config/system/stager/sync/' . $row->prefix);
    $operations .= ' - ' . l('Remove', 'admin/config/system/stager/remove/' . $row->prefix);
    $record[] = $operations;
    $records[] = $record;
  }

  // Show a nice table with the stager site.
  if (!empty($records)) {
    $variables['header'] = array(t('Title'), t('Created'), t('Operations'));
    $variables['rows'] = $records;
    $output = theme('table', $variables);
  }

  return array('#markup' => $output);
}

/**
 * The site is in staging mode.
 */
function stager_info() {
  $output = '<p>' . t('You are working in staging mode.') . '</p>';
  $output .= '<p>' . l(t('Click here to leave staging mode'), 'stager/mode/live');
  return array('#markup' => $output);
}

/**
 * Switch from staging to live mode or vica versa.
 *
 * @param $mode
 *   The name of the mode. Can either be staging or live.
 * @param $prefix
 *   The name of the prefix.
 */
function stager_switch_mode($mode = '', $prefix = '') {

  switch($mode) {

    case 'staging':
      if (stager_prefix_load($prefix)) {
        stager_set_cookie($prefix);
        drupal_goto('stager/action/enter_staging');
      }
      break;

    case 'live':
      stager_set_cookie();
      drupal_goto('stager/action/leave_staging');
      break;
  }


  // Bail out if nothing is found.
  drupal_set_message(t('The mode or the prefix are not found.'));
  drupal_goto('admin/config/system/stager');
}

/**
 * Create staging form.
 */
function stager_create_staging($form) {

  $form['info'] = array(
    '#markup' => '<p>' . t('No stager site has been created. Click on the on the button underneath to create one. Note that this might take a while.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create a new stager site'),
  );

  return $form;
}

/**
 * Create staging form submit callback.
 */
function stager_create_staging_submit($form, &$form_state) {

  $stager = new stdClass();
  $stager->prefix = 'stager';
  $stager->title = 'Stager';
  $stager->uid = $GLOBALS['user']->uid;
  $stager->created = REQUEST_TIME;
  drupal_write_record('stager', $stager);

  // Log action.
  stager_log('Staging site ' . $stager->prefix . ' has been created.');

  // Enter staging mode.
  stager_set_cookie($stager->prefix);

  // Create staging site.
  stager_staging_create($stager->prefix);

  // Redirect so cookie takes effect and the right message is shown.
  $form_state['redirect'] = 'stager/action/create_staging';
}

/**
 * Sync changes confirm form.
 */
function stager_sync_staging_confirm($form, $form_state, $prefix = '') {
  if (stager_prefix_load($prefix)) {
    $form['prefix'] = array('#type' => 'value', '#value' => $prefix);
    return confirm_form($form, t('Are you sure you want to sync changes from the staging version to the live site ? All data on the live site will be overwritten.'), 'admin/config/system/stager');
  }

  // Bail out if prefix is not found.
  drupal_set_message(t('Prefix not found'));
  drupal_goto('admin/config/system/stager');
}

/**
 * Sync changes confirm submit callback.
 */
function stager_sync_staging_confirm_submit($form, &$form_state) {

  $prefix = $form_state['values']['prefix'];

  // Perform operation.
  stager_staging_sync_and_or_remove($prefix, TRUE, FALSE);

  // Log action.
  stager_log('Staging site has been synchronized.');

  $form_state['redirect'] = 'admin/config/system/stager';
}

/**
 * Remove staging confirm form.
 */
function stager_remove_staging_confirm($form, $form_state, $prefix = '') {
  if ($stager_site = stager_prefix_load($prefix)) {
    $form['prefix'] = array('#type' => 'value', '#value' => $prefix);
    return confirm_form($form, t('Are you sure you want to remove %title ?', array('%title' => $stager_site->title)), 'admin/config/system/stager');
  }

  // Bail out if prefix is not found.
  drupal_set_message(t('Prefix not found'));
  drupal_goto('admin/config/system/stager');
}

/**
 * Remove staging confirm submit callback.
 */
function stager_remove_staging_confirm_submit($form, &$form_state) {

  $prefix = $form_state['values']['prefix'];

  // Remove database entry
  db_delete('stager')
    ->condition('prefix', $prefix)
    ->execute();

  // Perform operation.
  stager_staging_sync_and_or_remove($prefix, FALSE, TRUE);

  // Log action.
  stager_log('Stager site ' . $prefix . ' has been removed.');

  $form_state['redirect'] = 'admin/config/system/stager';
}
