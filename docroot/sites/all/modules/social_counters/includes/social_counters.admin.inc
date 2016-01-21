<?php
/**
 * @file
 * Administrative form for Social Counters.
 */

/**
 * Builds form for drupal_get_form() call in social_counters_admin_menu().
 */
function social_counters_admin_form($form, $form_state) {
  // Get counters.
  $counters = social_counters_get_counters();
  $delimiter = variable_get('social_counters_delimiter', ': ');
  $delimiter = social_counters_format_delimiter($delimiter);
  $output_example = social_counters_format_number(12345);

  // General settings.
  $form['settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General settings'),
    '#collapsible' => TRUE,
  );

  $form['settings']['social_counters_status'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled Social Counters:'),
    '#description' => t('Choose which Social Counters should be enabled.'),
    '#options' => social_counters_status_options($counters),
    '#default_value' => variable_get('social_counters_status', array()),
  );

  $form['settings']['social_counters_settings_expire'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Expire time'),
    '#description'   => t("The number of seconds social counters should be cached. Default is 21600 seconds(6 hours)."),
    '#default_value' => variable_get('social_counters_settings_expire', 21600),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['settings']['social_counters_delimiter'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Delimiter in output'),
    '#description'   => t("Delimiter between Social network label and number. Example of output: Facebook!delimiter @example", array(
      '!delimiter' => $delimiter, '@example' => $output_example)),
    '#default_value' => $delimiter,
  );

  $form['settings']['social_counters_short_format_number'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Short format for number'),
    '#description'   => t("Short format for number. For example 12345678 becomes 12.3M."),
    '#default_value' => variable_get('social_counters_short_format_number', FALSE),
  );

  foreach ($counters as $social_network => $data) {
    $form[$social_network] = array(
      '#type'        => 'fieldset',
      '#title'       => $data['title'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Check if any dependency is missed.
    if ($missings = social_counters_missing_dependencies($data)) {
      $form[$social_network]['missing'] = array(
        '#type' => 'item',
        '#title' => t('Missing dependencies:'),
        '#markup' => theme('item_list', array('items' => $missings)),
      );
      continue;
    }

    $base = 'social_counters_' . $social_network;
    // Show necessary fields for each social network.
    foreach ($data['variables'] as $var => $data) {
      $name = $base . '_' . $var;
      $form[$social_network][$name] = array(
        '#type'          => 'textfield',
        '#title'         => $data['title'],
        '#description'   => !empty($data['description']) ? $data['description'] : '',
        '#default_value' => variable_get($name, ''),
        '#maxlength' => 256,
      );
    }
  }

  return system_settings_form($form);
}

/**
 * Check if all dependencies for social network plugin have been already installed.
 *
 * @param array $data
 *   Data for social counter which is defined in hook_social_counters().
 *
 * @return array
 *    An array of missing dependencies.
 */
function social_counters_missing_dependencies($data) {
  $output = array();

  if (empty($data['dependencies'])) {
    return array();
  }

  foreach ($data['dependencies'] as $dependancy => $error) {
    if (!module_exists($dependancy)) {
      $output[] = $error;
    }
  }

  return $output;
}

/**
 * Return options for status in administrative interface.
 *
 * @param array $counters
 *   Array of available counters.
 *
 * @return array
 *   Array of status options.
 */
function social_counters_status_options($counters) {
  $options = array();

  foreach ($counters as $social_network => $counter) {
    $options[$social_network] = $counter['title'];
  }

  return $options;
}
