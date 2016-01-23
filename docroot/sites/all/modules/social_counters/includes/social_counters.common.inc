<?php
/**
 * @file
 * Common functions for Social Counters.
 */

/**
 * Get social counters output.
 *
 * @return array
 *   Array with output for enabled social networks.
 */
function social_counters_counters_output() {
  $items = array();
  $counters = social_counters_get_counters();
  // Leave only enabled Social Counters.
  $list = array_filter(variable_get('social_counters_status', array()));
  $counters = array_intersect_key($counters, $list);

  foreach ($counters as $social_network => $data) {
    $number = social_counters_get_number($social_network, $data);

    $items[$social_network] = array(
      'title' => check_plain($data['title']),
      'number' => social_counters_format_number($number),
    );
  }

  return $items;
}

/**
 * Get numbers of subscribers for specific social network.
 *
 * @param string $social_network
 *   Social network name(facebook, twitter, etc.).
 * @param array $data
 *    Data for social network plugin.
 *
 * @return int
 *   Number of subscribers.
 */
function social_counters_get_number($social_network, $data) {
  $id = variable_get('social_counters_' . $social_network . '_id', NULL);
  $expire = variable_get('social_counters_settings_expire', 21600);
  $number = FALSE;

  // Try to get number from cache.
  $cache_bin = 'social_counters:' . $social_network . ':' . $id;
  if ($cache = cache_get($cache_bin)) {
    return $cache->data;
  }

  // Pull data from social network.
  $function = $data['callback'];

  if (function_exists($function)) {
    $variables = social_counters_get_variables($social_network, array_keys($data['variables']));
    $number = $function($variables);
  }

  // Save data in cache.
  if (is_numeric($number)) {
    cache_set($cache_bin, $number, 'cache', REQUEST_TIME + $expire);
  }

  return $number;
}

/**
 * Get data from variables for specific social counter.
 *
 * @param string $social_network
 *   Social network name(facebook, twitter, etc.).
 * @param array $var_names
 *   Array of names of variables for social network.
 *
 * @return array
 *   Array of variables with filled values.
 */
function social_counters_get_variables($social_network, $var_names) {
  $variables = array();
  $base = 'social_counters_' . $social_network;
  foreach ($var_names as $name) {
    $variables[$name] = variable_get($base . '_' . $name);
  }
  $variables['social_network'] = $social_network;

  return $variables;
}

/**
 * Formates short output for numbers.
 *
 * @param int $number
 *   Number to be formatted.
 * @param int $precision
 *   Number of characters after the dot.
 *
 * @return string
 *   Number in short format.
 */
function social_counters_short_format_number($number, $precision = 1) {
  $thousand = 1000;
  $million = 1000000;

  if ($number < $thousand) {
    $formatted = $number;
  }
  elseif ($number < $million) {
    $formatted = number_format($number / $thousand, $precision) . 'K';
  }
  else {
    $formatted = number_format($number / $million, $precision) . 'M';
  }

  return $formatted;
}

/**
 * Return number in necessary format.
 *
 * @param int $number
 *   Number to be formatted.
 *
 * @return string
 *   Number in necessary format.
 */
function social_counters_format_number($number) {
  $result = $number;
  // Check if short format of number is enabled.
  if (variable_get('social_counters_short_format_number', FALSE)) {
    $result = social_counters_short_format_number($result);
  }

  return $result;
}

/**
 * Process delimiter to make it save for output.
 *
 * @param string $delimiter
 *   Delimiter to process.
 *
 * @return string
 *   Processed delimiter.
 */
function social_counters_format_delimiter($delimiter) {
  return filter_xss_admin($delimiter);
}
