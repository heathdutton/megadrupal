<?php

/**
 * @file
 * Administration callback functions.
 */

/**
 * Form constructor function for the admin settings form.
 */
function breadcrumbs_by_path_admin_settings_form($form, $form_state) {
  $form['breadcrumbs_by_path_current_title'] = array(
    '#title' => t('Add current title'),
    '#description' => t('Append the current page title to the end of this breadcrumb as some non-clickable text.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('breadcrumbs_by_path_current_title', 0),
  );

  $form['breadcrumbs_by_path_exclude_paths'] = array(
    '#title' => t('Paths to exclude'),
    '#description' => t('No breadcrumb will be displayed on these paths. Enter each path on a new line.'),
    '#type' => 'textarea',
    '#default_value' => variable_get('breadcrumbs_by_path_exclude_paths'),
  );

  $form['breadcrumbs_by_path_core_paths'] = array(
    '#title' => t('Paths to use core breadcrumbs'),
    '#description' => t('Enter each path on a new line.'),
    '#type' => 'textarea',
    '#default_value' => variable_get('breadcrumbs_by_path_core_paths'),
  );

  return system_settings_form($form);
}
