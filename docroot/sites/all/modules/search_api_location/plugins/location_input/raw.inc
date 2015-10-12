<?php

/**
 * @file
 * Plugin to provide raw location input.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Raw input'),
  'description' => t('Let user enter a location as decimal latitude and longitude, separated by a comma.'),
  'input callback' => 'search_api_location_input_raw',
);

/**
 * Returns the data exactly as entered.
 *
 * @param $input
 *   The text entered by the user.
 * @param array $options
 *   The options for this plugin (empty).
 *
 * @return
 *   $input if it is a valid location string. NULL otherwise.
 */
function search_api_location_input_raw($input, array $options) {
  $input = trim($input);
  return preg_match('/^[+-]?[0-9]+(?:\.[0-9]+)?,[+-]?[0-9]+(?:\.[0-9]+)?$/', $input) ? $input : NULL;
}
