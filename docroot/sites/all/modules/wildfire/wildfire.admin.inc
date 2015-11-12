<?php
/**
 * @file
 *  wildfire.admin.inc
 *
 * Administrative pages for the Wildfire module.
 *
 * @author Craig Jones <craig@tiger-fish.com>
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Main overview page for Wildfire.
 */
function wildfire_overview_page() {

  /**
   * Add in notices for cases where the child modules aren't enabled.
   */
  $required_modules = array(
    'wildfire_jobs',
    'wildfire_lists',
    'wildfire_messages',
    'wildfire_tracking'
  );

  foreach ($required_modules as $module) {
    if (!module_exists($module)) {
      drupal_set_message(
        t(
          'We HIGHLY recommend that the %module module is enabled.',
          array(
             '%module' => $module,
          )
        ),
        'warning'
      );
    }
  }

  $output = '';
  $blocks = module_invoke_all('wildfire_admin_block');

  $output .= theme('wildfire_admin_page', array('form' => $blocks));

  return $output;
}

/**
 * General settings form.
 */
function wildfire_settings_form($form, &$form_state) {

  $form = array();

  $form['system_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('System Settings'),
  );
  $form['system_fieldset']['wildfire_system_name'] = array(
    '#type' => 'textfield',
    '#title' => t('System Name'),
    '#required' => TRUE,
    '#description' => t('The machine-name for this Wildfire installation. You may use alphanumeric characters and hyphens.'),
    '#default_value' => variable_get('wildfire_system_name', ''),
  );

  return system_settings_form($form);
}

/**
 * Validate function for the settings form
 *
 * @param array $form
 * @param array $form_state
 */
function wildfire_settings_form_validate($form, &$form_state) {
  // Check that no illegal characters are in the system name.
  $regex = '/^[a-zA-Z0-9\-]+$/';
  $valid = preg_match($regex, $form_state['values']['wildfire_system_name']);

  if ($valid !== 1) {
    form_set_error(
      'wildfire_system_name',
      t('You are only permitted use to alphanumeric characters and hyphens in the system name.')
    );
  }

}

/**
 * Provide a statistics block for the Wildfire overview page.
 */
function wildfire_stats_admin_block() {

  $stats = array();

  if (module_exists('wildfire_lists')) {

    module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

    // Get the number of lists.
    $lists = wildfire_lists_get();

    $stats[] = array(
      'label' => t('Lists'),
      'value' => count($lists),
    );

  }

  // Get the total number of users.
  $query = db_select('users', 'u');
  $query->addExpression('COUNT(1)', 'count');
  $result = $query->execute();

  $user_count = $result->fetchField();

  $stats[] = array(
    'label' => t('Total users'),
    'value' => $user_count,
  );

  // Put the stats into a table.
  $rows = array();

  foreach ($stats as $stat) {
    $row = array();

    $row[] = $stat['label'];
    $row[] = $stat['value'];

    $rows[] = $row;
  }

  $content = theme('table', array('header' => array(), 'rows' => $rows));

  return array(
    'title' => t('Statistics'),
    'description' => t('The latest statistics for your system.'),
    'content' => $content,
    'show' => TRUE,
  );
}
