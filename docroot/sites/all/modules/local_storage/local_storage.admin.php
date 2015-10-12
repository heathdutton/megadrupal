<?php

/**
 * @file
 * Contains form callback for admin page.
 */

/**
 * Form callback for "admin/config/administration/local_storage".
 */
function local_storage_admin_form($form, &$form_state) {
  $form['local_storage_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Local Storage'),
    '#description' => t(
      'Enable automatic storing of entered data for all new fields by default.'
    ),
    '#default_value' => variable_get('local_storage_enable', 0),
  );

  $form['local_storage_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show default (original) value by default'),
    '#description' => t(
      'Show default (original) value by default for all new fields by default.'
    ),
    '#default_value' => variable_get('local_storage_default', 0),
  );

  $form['local_storage_expire'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 48)),
    '#title' => t('Expiration time'),
    '#description' => t(
      'Set default expiration time for stored data (in hours).'
    ),
    '#default_value' => variable_get('local_storage_expire', 48),
  );

  return system_settings_form($form);
}

/**
 * Submit callback for "local_storage_admin_form".
 */
function local_storage_admin_form_submit() {
  cache_clear_all('*', 'cache', TRUE);
}
