<?php
/**
 * @file
 * DBinfo administration pages.
 */

/**
 * Administration page settings form for Dbinfo.
 */
function dbinfo_settings($form, &$form_state) {
  $form['dbinfo_db_test_connections'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Connection Tests within Status Report'),
    '#description' => t('Attempt to connect to every database and target defined in settings.php from the status report page.'),
    '#default_value' => variable_get('dbinfo_db_test_connections', FALSE),
    '#suffix' => '<br\>',
  );
  return system_settings_form($form);
}
