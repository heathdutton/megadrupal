<?php

/**
 * @file
 * Migration form for converting from raw_pages module.
 */

/**
 * Builds form with all raw_pages as options.
 */
function pathed_files_migrate_raw_pages($form, &$form_state) {
  $form = array();

  $page_id = 1;
  while ($path = variable_get('raw_pages_' . $page_id . '_path')) {
    $options[$page_id] = l($path, $path, array('attributes' => array('target' => '_blank')));
    $page_id++;
  }

  $form['pages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Which pages would you like to migrate?'),
    '#description' => t('Each selected page will be migrated to be a
      pathed_file entry.'),
    '#options' => $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate'),
  );

  return $form;
}

/**
 * Migration form: Validation.
 */
function pathed_files_migrate_raw_pages_validate($form, &$form_state) {
  $selected = array_filter($form_state['values']['pages']);
  if (empty($selected)) {
    form_set_error('pages', t('Please select a raw page to continue.'));
  }
}

/**
 * Migration form: Submission.
 *
 * Hands off the heavy lifting to form pathed_files_admin_edit.
 */
function pathed_files_migrate_raw_pages_submit($form, &$form_state) {
  module_load_include('inc', 'pathed_files', 'pathed_files.admin');

  $raw_page_ids = array_filter($form_state['values']['pages']);

  foreach ($raw_page_ids as $id) {

    $path = variable_get('raw_pages_' . $id . '_path');
    $pathed_file_form_state['values'] = array(
      'file_name' => $path,
      'file_description' => $path,
      'file_uri_path' => $path,
      'use_file_content' => 1,
      'new_file_id' => 0,
      'file_content' => variable_get('raw_pages_' . $id . '_content'),
      'enabled_flag' => 1,
    );
    $pathed_file_form_state['build_info']['args'] = array('migrated');
    drupal_form_submit(
      'pathed_files_admin_edit',
      $pathed_file_form_state
    );
  }
}
