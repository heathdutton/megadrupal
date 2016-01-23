<?php

/**
 * @file
 * Plugin to provide location input using geocoder.
 */

if (module_exists('geocoder')) {
  /**
   * Plugins are described by creating a $plugin array which will be used
   * by the system that includes this file.
   */
  $plugin = array(
    'title' => t('Geocoder input'),
    'description' => t('Use any Geocoder location input method.'),
    'input callback' => 'search_api_location_input_geocoder',
    'form callback' => 'search_api_location_input_geocoder_form',
  );
}

/**
 * Uses geocoder to parse user input into a location.
 *
 * @param $input
 *   The text entered by the user.
 * @param array $options
 *   The options for this plugin. See search_api_location_input_geocoder_form().
 *
 * @return
 *   A location if the geocoding succeeded, NULL otherwise.
 */
function search_api_location_input_geocoder($input, array $options) {
  $options += array(
    'geocoder_handler' => NULL,
    'geocoder_handler_options' => array(),
  );
  $location = geocoder($options['geocoder_handler'], $input, $options['geocoder_handler_options']);
  if ($location) {
    $location = $location->centroid();
    return $location->y() . ',' . $location->x();
  }
}

/**
 * Provides a configuration form for this location input plugin.
 *
 * @param array $form_state
 *   The form state data.
 *
 * @return array
 *   A form array for configuring this location input plugin.
 */
function search_api_location_input_geocoder_form(array &$form_state, array $options) {
  $options += array(
    'geocoder_handler' => NULL,
    'geocoder_handler_options' => array(),
  );

  $handler_id = NULL;
  $processor = NULL;
  if (!empty($form_state['values'])) {
    $handler_id = _search_api_location_array_get_nested_value($form_state['values'], 'geocoder_handler');
  }
  $handler_id = $handler_id ? $handler_id : $options['geocoder_handler'];
  if ($handler_id) {
    $processor = ctools_get_plugins('geocoder', 'geocoder_handler', $handler_id);
  }

  $handlers = array();
  foreach (geocoder_handler_info() as $id => $handler) {
    $handlers[$id] = $handler['title'];
  }
  $form['geocoder_handler'] = array(
    '#type' => 'select',
    '#title' => t('Handler'),
    '#description' => t('Select the input format for Geocoder.'),
    '#options' => $handlers,
    '#default_value' => $options['geocoder_handler'],
    '#ajax' => array(
      'callback' => 'search_api_location_input_geocoder_form_ajax',
      'wrapper' => 'search-api-location-handler-options',
    ),
  );

  if ($processor) {
    $form['geocoder_handler_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Handler options'),
      '#tree' => TRUE,
      '#prefix' => '<div id="search-api-location-handler-options">',
      '#suffix' => '</div>',
    );
    if (!empty($processor['description'])) {
      $form['geocoder_handler_options']['#description'] = $processor['description'];
    }
    if (!empty($processor['settings_callback'])) {
      $handler_options = $processor['name'] == $options['geocoder_handler'] ? $options['geocoder_handler_options'] : array();
      $options_form = $processor['settings_callback']($handler_options);
      if ($options_form) {
        $form['geocoder_handler_options'] += $options_form;
      }
    }
  }
  else {
    $form['geocoder_handler_options']['#markup'] = '<div id="search-api-location-handler-options"></div>';
  }

  return $form;
}

/**
 * AJAX callback for displaying the Geocoder handler options.
 */
function search_api_location_input_geocoder_form_ajax(array $form, array &$form_state) {
  return _search_api_location_array_get_nested_value($form, 'geocoder_handler_options');
}
