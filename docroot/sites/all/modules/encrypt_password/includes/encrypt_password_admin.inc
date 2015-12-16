<?php

/**
 * @file
 * Provide administrative functions for managing settings for
 * encrypting user passwords.
 */

/**
 * Form for managing settings.
 */
function encrypt_password_settings_form($form, &$form_state) {
  $roles = user_roles(TRUE, NULL);
  $build_table = encrypt_password_theme_encrypted_counts();

  $form['encrypt_password_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#attributes' => array(
      'class' => array(
        'encrypt-password-settings',
      ),
    ),
  );
  $form['encrypt_password_settings']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Encrypt passwords'),
    '#default_value' => _encrypt_password_settings_enabled(),
  );
  $form['encrypt_password_settings']['config'] = array(
    '#type' => 'select',
    '#title' => t('Configuration'),
    '#description' => t('Select a configuration to be used for password encryption.'),
    '#options' => encrypt_get_enabled_configs_as_options(),
    '#default_value' => _encrypt_password_settings_config(),
    '#states' => array(
      'disabled' => array(
        ':input[name="encrypt_password_settings[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['encrypt_password_settings']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Roles',
    '#options' => $roles,
    '#description' => t("Passwords will be encrypted for users with the selected roles. Check 'authenticated user' to encrypt all passwords."),
    '#default_value' => _encrypt_password_settings_roles(),
    '#states' => array(
      'disabled' => array(
        ':input[name="encrypt_password_settings[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['encrypt_password_statistics'] = array(
    '#type' => 'fieldset',
    '#title' => 'Statistics',
    '#collapsible' => TRUE,
    '#attributes' => array(
      'class' => array(
        'encrypt-password-statistics',
      ),
    ),
  );
  $form['encrypt_password_statistics']['statistics'] = array(
    '#type' => 'markup',
    '#markup' => drupal_render($build_table),
  );

  $form['#submit'][] = 'encrypt_password_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for the settings form.
 */
function encrypt_password_settings_form_submit($form, &$form_state) {
  $settings = $form_state['values']['encrypt_password_settings'];

  // Clean up roles.
  $settings['roles'] = _encrypt_password_parse_roles($settings['roles']);
  encrypt_password_user_password_update($settings);
  encrypt_password_bulk_password_update($settings);
}

/**
 * Update the logged-in user.
 *
 * This is needed because a "No active batch" error is displayed if the
 * logged-in user is part of the bulk update.
 */
function encrypt_password_user_password_update($settings) {
  global $user;

  $account = user_load($user->uid, FALSE);
  $user = $account;

  $edit['encrypt_password']['settings'] = $settings;
  user_save($user, $edit);
}

/**
 * Perform the bulk update.
 */
function encrypt_password_bulk_password_update($settings) {
  $batch = array(
    'operations' => array(
      array('encrypt_password_batch_process', array($settings)),
    ),
    'file' => drupal_get_path('module', 'encrypt_password') . '/includes/encrypt_password_batch.inc',
    'finished' => 'encrypt_password_batch_finished',
    'title' => t('Processing user passwords'),
  );
  batch_set($batch);
}

/**
 * Helper function to theme encryption statistics.
 *
 * @return string
 *   The themed encryption statistics.
 */
function encrypt_password_theme_encrypted_counts() {
  $rows = array();
  $roles = user_roles(TRUE, NULL);
  $header = array(
    t('Role'),
    t('Encrypted'),
    t('Not Encrypted'),
    t('No Password'),
  );

  foreach ($roles as $rid => $role) {
    $row = array(
      array('data' => $role),
      array('data' => _encrypt_password_user_count('encrypted', $rid)),
      array('data' => _encrypt_password_user_count('unencrypted', $rid)),
      array('data' => _encrypt_password_user_count('no_password', $rid)),
    );

    $rows[] = $row;
  }

  $build['encrypt_password_count_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No password information is available.'),
  );

  return $build;
}
