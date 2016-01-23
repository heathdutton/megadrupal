 <?php
/**
 * @file
 * Debugging functions.
 */


/**
 * Setting form for the QD Debug functionality.
 *
 * @return array
 *   The settings form array.
 */
function qd_core_debug_settings_form() {
  $form = array();

  $form['qd_core_use_cached_calls'] = array(
    '#type' => 'textfield',
    '#title' => t('Use cached calls'),
    '#default_value' => variable_get('qd_core_use_cached_calls'),
    '#type' => 'checkbox',
    '#description' => t('Using cached calls means that sample web service calls are loaded from the disk instead of making live calls out to live web services. This enables local development and ensures you don\'t get rate-limited while developing.'),

  );

  return system_settings_form($form);
}


/**
 * Form for running debug commands.
 *
 * @return array
 *   The form array.
 */
function qd_core_debug_run_form() {
  $form = array();

  if (module_exists('devel')) {
    $form['qd_core']['delete'] = array(
      '#type' => 'fieldset',
      '#title' => t('Delete data'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // @TODO: Only allow tables that have been created to be truncated.
    $form['qd_core']['delete']['qd_core_truncate_tables'] = array(
      '#type' => 'checkboxes',
      '#options' => array(
        'qd_core_activitylevel' => t('qd_core_activitylevel'),
        'qd_core_bmi' => t('qd_core_bmi'),
        'qd_core_bodyfat' => t('qd_core_bodyfat'),
        'qd_core_distance' => t('qd_core_distance'),
        'qd_core_elevation' => t('qd_core_elevation'),
        'qd_core_steps' => t('qd_core_steps'),
        'qd_core_weight' => t('qd_core_weight'),
      ),
      '#title' => t('Delete all data from'),
      '#default_value' => array('qd_core_activitylevel', 'qd_core_bmi', 'qd_core_bodyfat', 'qd_core_distance', 'qd_core_elevation', 'qd_core_steps', 'qd_core_weight'),
    );

    $form['qd_core']['delete']['qd_core_truncate_button'] = array(
      '#type' => 'submit',
      '#value' => 'Delete data',
      '#submit' => array('qd_core_truncate_tables'),
    );

    $form_addons = module_invoke_all('qd_debug_form');

    $form[] = $form_addons;
  }
  else {
    drupal_set_message('Enable the devel module to use this page.');
  }

  return $form;
}

/**
 * Submission callback - truncates tables.
 *
 * @param array $form
 *   The submitted form array.
 * @param array $form_state
 *   The values for the submitted form array.
 */
function qd_core_truncate_tables($form, $form_state) {
  $tables = $form_state['values']['qd_core_truncate_tables'];

  foreach ($tables as $table) {
    if ($table != '0') {
      db_truncate($table)->execute();
      drupal_set_message($table . ' truncated');
    }
  }
}
