<?php

/**
 * @file
 * Hooks provided by the Search API location module.
 */

/**
 * Defines plugins for handling location input.
 *
 * This can be used to allow users different methods to enter location data.
 * This module already provides two such plugins: "raw" for raw input of
 * locations as coordinates, and "geocoder" (if the geocoder module is enabled)
 * for location input handled by Geocoder.
 *
 * This uses CTools' plugin system, so plugins can also be defined via plugin
 * files. For details, see CTools' documentation (if you have advanced_help
 * installed: help/ctools/plugins-implementing), or look at the files in
 * plugins/location_input in this module for examples.
 *
 * Also see CTools' documentation for other hooks provided for these plugins.
 *
 * @return array
 *   An array of location_input plugin definitions. The keys are the plugins'
 *   identifiers, the values are arrays with the following keys:
 *   - title: The human-readable name of the plugin.
 *   - description: (optional) A human-readable description of how input is
 *     handled by this plugin.
 *   - input callback: Callback to parse user input into a location. For the
 *     function documentation of the callback, see
 *     example_search_api_location_input_callback() below.
 *   - form callback: (optional) A callback for showing a configuration form for
 *     this plugin to the user. For the function documentation of the callback,
 *     see example_search_api_location_form_callback() below.
 */
function hook_search_api_location_location_input() {
  $plugins['example'] = array(
    'title' => t('Example location thingie'),
    'description' => t('Determine location by higher magic.'),
    'input callback' => 'example_search_api_location_input_callback',
    'form callback' => 'example_search_api_location_form_callback',
  );
  return $plugins;
}

/**
 * Alter the possible units offered by the Search API location module.
 *
 * @param array $units
 *   An array of units, where the keys are the conversion factors to kilometers
 *   and the values are the unit names.
 */
function hook_search_api_location_units_alter(array &$units) {
  $units = array(
    '1' => t('Kilometers'),
    '0.001' => t('Meters'),
    '0.0000254' => t('Inches'),
  );
}

/**
 * Example for a location_input plugin input callback.
 *
 * @param $input
 *   The text entered by the user.
 * @param array $options
 *   The options for this plugin, as entered into the form specified by the
 *   "form callback".
 *
 * @return
 *   A location if the input could be parsed, NULL otherwise. The location has
 *   to be a string in the format "LAT,LON", where LAT and LON are the decimal
 *   latitude and longitude, respectively, of the location.
 */
function example_search_api_location_input_callback($input, array $options) {
  $location = geocoder($options['handler'], $input);
  if ($location) {
    $location = $location->centroid();
    return $location->y() . ',' . $location->x();
  }
}

/**
 * Example for a location_input plugin form callback.
 *
 * @param array $form_state
 *   The form state data.
 * @param array $options
 *   The options for this plugin, as entered into this form previously.
 *
 * @return array
 *   A form array for configuring this location_input plugin. Upon submitting of
 *   the configuration form, the resulting $form_state['values'] will
 *   automatically be saved as the $options of this plugin for future
 *   invocations.
 */
function example_search_api_location_form_callback(array &$form_state, array $options) {
  $handlers = array();
  foreach (geocoder_handler_info() as $id => $handler) {
    $handlers[$id] = $handler['title'];
  }
  $form['handler'] = array(
    '#title' => t('Handler'),
    '#type' => 'select',
    '#options' => $handlers,
    '#default_value' => isset($options['handler']) ? $options['handler'] : NULL,
  );

  return $form;
}