<?php
/**
 * @file
 * Controls the common functions used by DEVICE DETECTOR module.
 */

/**
 * Returns the settings classes for the current url.
 *
 * @param string $current_path
 *   The current url path.
 *
 * @return string $nodes_class_names
 *   Returns the settings classes corresponding to the current url.
 */
function device_detector_check_url_and_return_classes($current_path) {
  $nodes_class_names = '';
  $device_detector_node_urls = drupal_strtolower(variable_get('device_detector_node_urls'));
  $node_url_array = array_map('trim', explode("\n", $device_detector_node_urls));
  $node_url_mapped_class_array = array_map('trim', explode("\n", variable_get('device_detector_node_class_names')));
  if (drupal_match_path($current_path, $device_detector_node_urls)) {
    foreach ($node_url_array as $key => $path) {
      // Compare the lowercase internal and lowercase path alias (if any)
      // Check with different case. Ex: /Page, /page, /PAGE.
      if (drupal_match_path($current_path, $path) && array_key_exists($key, $node_url_array)) {
        $nodes_class_names .= ' ' . trim(str_replace(',', ' ', $node_url_mapped_class_array[$key]));
      }
    }
  }
  return $nodes_class_names;
}

/**
 * Returns the browser informations.
 *
 * @return array $browser_details
 *   Returns an array containing the browser name and it version(integer part).
 */
function device_detector_get_browser_info() {
  // Variable declaration and initialisation.
  // Getting the server user agent.
  $user_agent = $_SERVER['HTTP_USER_AGENT'];
  // Browser details container array.
  $browser_details = array();
  // Allowed browser tracker info.
  $browsers = array(
    'Chrome' => array(DEVICE_DETECTOR_CHROME_NAME, 'Chrome/(.*)\s'),
    'Firefox' => array(DEVICE_DETECTOR_FIREFOX_NAME, 'Firefox/([0-9\.]*)'),
    'MSIE' => array(DEVICE_DETECTOR_MSIE_NAME, 'MSIE\s([0-9\.]*)'),
    'Trident' => array(DEVICE_DETECTOR_MSIE_NAME, 'Trident/([07-9\.]*?);\s?rv:([01\.]*)'),
    'Safari' => array(DEVICE_DETECTOR_SAFARI_NAME, 'Version/([0-9\.]*)'),
    'Opera' => array(DEVICE_DETECTOR_OPERA_NAME, 'Version/([0-9\.]*)'),
  );
  // Checking and extracting browser info if finds a match.
  foreach ($browsers as $browser => $browser_info) {
    if (preg_match('@' . $browser . '@i', $user_agent)) {
      $browser_details['browser'] = $browser_info[0];
      preg_match('@' . $browser_info[1] . '@i', $user_agent, $version);
      $browser_details['version'] = ($browser != 'Trident') ? ((int) $version[1]) : ((int) $version[2]);
      break;
    }
  }
  return $browser_details;
}

/**
 * Returns the start occurrence of ',' character.
 *
 * @return bool
 *   Returns TRUE/FALSE.
 */
function device_detector_str_starts_with($haystack, $needle) {
  // Search backwards starting from haystack from the end.
  return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== FALSE;
}

/**
 * Returns the end occurrence of ',' character.
 *
 * @return bool
 *   Returns TRUE/FALSE.
 */
function device_detector_str_ends_with($haystack, $needle) {
  // Search forward starting from end minus needle length characters.
  return $needle === "" || (($temp = strlen($haystack) - strlen($needle)) >= 0 && strpos($haystack, $needle, $temp) !== FALSE);
}
