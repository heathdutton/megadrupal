<?php

/**
 * @file
 * Administration related functionality for the module.
 */

/**
 * Form builder for the entities short codes settings.
 *
 * @ingroup forms
 */
function variable_log_settings_form($form, $form_state) {
  $form['variable_log_cleanup_type'] = array(
    '#type' => 'select',
    '#title' => t('Database log storage method'),
    '#options' => array(
      'row_limit' => t('Row limit'),
      'timestamp_limit' => t('Timestamp limit'),
    ),
    '#default_value' => variable_get('variable_log_cleanup_type', 'row_limit'),
  );

  $form['variable_log_row_limit'] = array(
    '#type' => 'select',
    '#title' => t('Database log messages to keep'),
    '#options' => array(0 => t('All')) + drupal_map_assoc(array(100, 1000, 10000, 100000, 1000000)),
    '#default_value' => variable_get('variable_log_row_limit', 1000),
    '#states' => array(
      'visible' => array(
        'select[name="variable_log_cleanup_type"]' => array('value' => 'row_limit'),
      ),
    ),
  );

  $form['variable_log_timestamp_limit'] = array(
    '#type' => 'select',
    '#title' => t('Delete log messages older than'),
    '#options' => array(0 => t('Never (do not discard)')) + drupal_map_assoc(array(604800, 1209600, 1814400, 2592000, 5184000, 7776000, 10368000, 15552000, 31536000), 'format_interval'),
    '#default_value' => variable_get('variable_log_timestamp_limit', 2592000),
    '#states' => array(
      'visible' => array(
        'select[name="variable_log_cleanup_type"]' => array('value' => 'timestamp_limit'),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Page callback: Displays diffs after variable update.
 *
 * @param int $id
 *   Unique ID of the database log.
 *
 * @return string
 *   If the ID is located in the Database, build diffs table, otherwise,
 *   MENU_NOT_FOUND.
 *
 * @see variable_log_menu()
 */
function variable_log_details_page_callback($id) {
  $output = MENU_NOT_FOUND;
  if ($result = variable_log_record_load($id)) {
    $title = t('Details for @name', array('@name' => $result['name']));
    drupal_set_title($title);

    $path = drupal_get_path('module', 'diff');
    drupal_add_css($path . '/css/diff.default.css');

    $old_value = unserialize($result['old_value']);
    $value = unserialize($result['value']);

    $diff = diff_get_rows(print_r($old_value, TRUE), print_r($value, TRUE));
    $output = theme('table', array('rows' => $diff));
  }

  return $output;
}
