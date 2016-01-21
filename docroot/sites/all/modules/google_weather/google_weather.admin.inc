<?php
/**
 * @file
 * Google weather module admin.inc.
 *
 */

/**
 * Form for location blocks settings page.
 */
function google_weather_location_admin_form() {
  $locations = _google_weather_get_all_locations();

  $rows = array();

  if ($locations) {
  // Add core ajax library.
  drupal_add_library('system', 'drupal.ajax');

  $token = drupal_get_token('google_weather_remove_location');

    foreach ($locations as $location) {
      $rows[$location->bid]['data'] = array(
        'location' => check_plain($location->location),
        'days' => $location->days > 1 ? $location->days -1 : t("Current only"),
        'block' => $location->block ? t("Yes") : t("No"),
        'edit' => array('data' => l(t("Edit"), "admin/config/services/google_weather/locations/$location->bid/edit")),
        'remove' => array('data' => l(t("Remove"), "admin/config/services/google_weather/locations/$location->bid/remove/nojs",
          array('query' => array('token' => $token), 'attributes' => array('class' => 'use-ajax')))),
      );
      $rows[$location->bid]['id'] = array("google-weather-row-$location->bid");
    }
  }

  $header = array('Location', 'Days', 'Render as block', array('data' => '', 'colspan' => 2));

  $empty = html_tag(t("No locations have been added yet."), 'p');

  $table_vars = array(
    'header' => $header,
    'rows' => $rows,
  );

  $form['google_weather'] = array(
    '#type' => 'fieldset',
  );
  $form['google_weather']['table'] = array(
    '#type' => 'markup',
    '#markup' => !empty($rows) ? theme('table', $table_vars) : $empty,
  );

  return system_settings_form($form);
}

/**
 * Form to create a new location.
 */
function google_weather_add_location_form() {

  $form['google_weather']['location'] = array(
    '#type' => 'textfield',
    '#title' => t("Location"),
    '#description' => t("The name or postal/zip code of the location to search for weather for."),
    '#required' => TRUE,
  );
  $form['google_weather']['no_of_days'] = array(
    '#type' => 'select',
    '#title' => t("Number of days"),
    '#description' => t("The number of days to show weather for in the block."),
      '#options' => array(
        '1' => t("Current only"),
        '2' => 1,
        '3' => 2,
        '4' => 3,
        '5' => 4,
    ),
    '#default_value' => 2,
  );
  $form['google_weather']['block'] = array(
    '#type' => 'checkbox',
    '#title' => t("Make block for location"),
    '#description' => t("If this checkbox is ticked, a block for this location will be available."),
    '#default_value' => TRUE,
  );
  $form['google_weather']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save location"),
  );

  $form['#tree'] = TRUE;

  return $form;
}

function google_weather_add_location_form_submit($form, &$form_state) {
  $values = $form_state['values']['google_weather'];

  $new = db_insert('google_weather_blocks')
    ->fields(array(
      'location' => $values['location'],
      'days' => $values['no_of_days'],
      'block' => $values['block'],
    ))
    ->execute();

  if ($new) {
    drupal_set_message(t("New location %location has been saved.",
      array('%location' => $values['location'])));
  }

  $form_state['redirect'] = array('admin/config/services/google_weather/locations');

  // Clear all caches when new block is added.
  cache_clear_all('*', 'cache_block', TRUE);
}

/**
 * Form for editing stored location information.
 */
function google_weather_edit_location_form($form, &$form_state, $location) {
  $form_state['google_weather_location'] = $location;

  $form['google_weather']['location'] = array(
    '#type' => 'textfield',
    '#title' => t("Location name"),
    '#description' => t("The name of the location to search for weather for."),
    '#default_value' => $location->location,
  );
  $form['google_weather']['no_of_days'] = array(
    '#type' => 'select',
    '#title' => t("Number of days"),
    '#description' => t("The number of days to show weather for in the block."),
      '#options' => array(
        '1' => t("Now only"),
        '2' => 1,
        '3' => 2,
        '4' => 3,
        '5' => 4,
      ),
    '#default_value' => $location->days,
  );
  $form['google_weather']['block'] = array(
    '#type' => 'checkbox',
    '#title' => t("Make block for location"),
    '#description' => t("If this checkbox is ticked, a block for this location will be available."),
    '#default_value' => $location->block,
  );
  $form['google_weather']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save location"),
  );

  $form['#tree'] = TRUE;

  return $form;
}

function google_weather_edit_location_form_submit($form, &$form_state) {
  $values = $form_state['values']['google_weather'];
  $bid = $form_state['google_weather_location']->bid;

  $new = db_update('google_weather_blocks')
    ->condition('bid', $bid)
    ->fields(array(
      'location' => $values['location'],
      'days' => $values['no_of_days'],
      'block' => $values['block'],
    ))
    ->execute();

  if ($new) {
    drupal_set_message(t("Location %location has been saved.",
      array('%location' => $values['location'])));
  }

  $form_state['redirect'] = array('admin/config/services/google_weather/locations');

  cache_clear_all('*', 'cache_block', TRUE);
}

/**
 * Form for global module settings page.
 */
function google_weather_settings_admin_form() {
  $form = array();

  $form['google'] = array(
    '#type' => 'fieldset',
    '#title' => t("Google configuration"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['google']['google_weather_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t("Connection timeout"),
    '#size' => 5,
    '#default_value' => variable_get('google_weather_timeout', 10),
    '#required' => TRUE,
  );
  $form['google_weather'] = array(
    '#type' => 'fieldset',
    '#title' => t("Local settings"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['google_weather']['google_weather_units'] = array(
    '#type' => 'select',
    '#title' => t("Unit system"),
    '#default_value' => variable_get('google_weather_units', 'SI'),
    '#options' => array(
      'SI' => t("SI (Celsius)"),
      'US' => t("US (Fahrenheit)"),
    ),
    '#required' => TRUE,
    '#description' => t("Global setting for the unit system"),
  );
  $form['google_weather']['google_weather_block_cache'] = array(
    '#type' => 'select',
    '#title' => t("Block cache lifetime"),
    '#default_value' => variable_get('google_weather_block_cache', 3600),
    '#options' => array(
      0 => t("None (performance issues)"),
      300 => t("5 mins"),
      600 => t("10 mins"),
      1800 => t("30 mins"),
      3600 => t("1 hour"),
      21600 => t("6 hours"),
      43200 => t("12 hours"),
      84600 => t("24 hours"),
    ),
    '#description' => t("The amount of time Google weather blocks will be cached
      before weather information is re-requested. Having this set to 0 will have performance issues
      (recommended for development sites only)."),
  );
  $form['google_weather']['google_weather_no_service'] = array(
    '#type' => 'textfield',
    '#title' => t("No service message"),
    '#description' => t("The message that will be displayed in blocks if the Google weather service cannot be requested."),
    '#default_value' => variable_get('google_weather_no_service',
      t("Data could not be requested from google weather service.")),
  );

  $form['#submit'][] = 'google_weather_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Clear caches in our own submit function when system settings are saved.
 * They will be cached otherwise.
 */
function google_weather_admin_form_submit($form, $form_state) {
  cache_clear_all('*', 'cache_block', TRUE);
}