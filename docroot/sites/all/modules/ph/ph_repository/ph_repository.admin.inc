<?php
/**
 * @file
 * Administrative UI implementation.
 */

/**
 * Repository settings form.
 */
function ph_repository_admin_settings($form, &$form_state) {
  $form['directory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Directory'),
  );
  $form['directory']['ph_repository_working_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Repository working directory'),
    '#default_value' => ph_repository_working_directory(),
    '#required' => TRUE,
    '#element_validate' => array('ph_repository_validate_directory'),
    '#description' => t('Specify a writable directory to check out repositories to export.'),
  );
  $form['directory']['help'] = array(
    '#markup' => theme('ph_repository_directory_help'),
    '#weight' => 5,
  );

  $form['repository'] = array(
    '#type' => 'fieldset',
    '#title' => t('General repository settings'),
  );
  $form['repository']['ph_repository_sync_auto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically synchronize repositories'),
    '#default_value' => variable_get('ph_repository_sync_auto', 1),
    '#description' => t('Check to synchronize a repository before performing an operation on it, e.g. listing versions and exporting packages. Leave this unchecked if repositories are synchronized through another mechanism, e.g. system cron job.'),
  );

  return system_settings_form($form);
}

/**
 * Builds help text for directory settings.
 */
function theme_ph_repository_directory_help($variables) {
  $output = '<p>';
  $output .= t('You may prefix the directory path with one of the following to indicate the associated path as configured in !file-system:', array(
    '!file-system' => user_access('administer site configuration') ? l(t('file system settings'), 'admin/config/media/file-system') : t('file system settings'),
  ));
  $output .= '</p>';

  $output .= '<dl>';
  $descriptions = array(
    'public://' => t('Public file system path.'),
    'private://' => t('Private file system path.'),
    'temporary://' => t('Temporary directory.'),
  );
  foreach ($descriptions as $path => $name) {
    $output .= '<dt>' . $path . '</dt>';
    $output .= '<dd>' . $name . '</dd>';
  }
  $output .= '</dl>';

  return $output;
}
