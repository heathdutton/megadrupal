<?php
/**
 * @file
 * Common functions for Social Counters.
 */

/**
 * Get social counters output.
 */
function social_counters_counters_output() {
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
 * @param social_network
 *   Social network name(facebook, twitter, etc.).
 * @param $data
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
 */
function social_counters_get_variables($social_network, $names) {
  $variables = array();
  $base = 'social_counters_' . $social_network;
  foreach ($names as $name) {
    $variables[$name] = variable_get($base . '_' . $name);
  }
  $variables['social_network'] = $social_network;

  return $variables;
}

/**
 * Formates short output for numbers.
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
 * Show delimiter in correct format.
 */
function social_counters_format_delimiter($delimiter) {
  return filter_xss_admin($delimiter);
}
