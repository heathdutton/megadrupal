<?php

/**
 * @file
 * helper functions. not only usefull for agreservations
 */

/**
 * _install_bundle_fields: inspired by features module.
 *
 */
function _install_bundle_fields($fields) {
  if (isset($fields)) {
    field_info_cache_clear();
    foreach ($fields as $field) {
      // Create or update field.
      $field_config = $field['field_config'];
      if ($existing_field = field_info_field($field_config['field_name'])) {
        field_update_field($field_config);
      }
      else {
        field_create_field($field_config);
      }

      // Create or update field instance.
      $field_instance = $field['field_instance'];
      $existing_instance = field_info_instance($field_instance['entity_type'], $field_instance['field_name'], $field_instance['bundle']);
      if ($existing_instance) {
        field_update_instance($field_instance);
      }
      else {
        field_create_instance($field_instance);
      }
      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}

/**
 * in_multi_assoc_array: returns BOOLEAN.
 *
 * @author     Andreas Gill
 * @param      $needle zu suchender Wert.
 * @param      $array   zu durchsuchendes Array.
 * @return     TRUE oder FALSE
 */
function in_multi_assoc_array($needle, $array) {
  foreach ($array as $element) {
    if ($element == $needle) {
      return TRUE;
    }

    if (is_array($element)) {
      if (in_multi_assoc_array($needle, $element)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * _is_time_in_range: returns BOOLEAN.
 *
 * @author     Andreas Gill
 * @param      $timetocheck time to check.timestring'H:i'
 * @param      $rangelow lower end of timerange.timestring'H:i'
 * @param      hrangehigh higher end of timerange.timestring of form 'H:i'
 * @return     TRUE if in range or FALSE if not within range
 */
function _is_time_in_range($timetocheck, $rangelow, $rangehigh) {
//drupal_set_message('<pre>_is_time_in_range_is_time_in_range '.print_r($timetocheck,true).'</pre>');
  $strtimetocheck = date('H:i', strtotime($timetocheck));
  if ((strtotime($strtimetocheck) >= strtotime($rangelow)) && (strtotime($strtimetocheck) <= strtotime($rangehigh))) {
    return TRUE;
  }
  return FALSE;
}

/**
 * _is_date_in_range: returns BOOLEAN. *
 * @author     Andreas Gill
 * @param      $datetocheck, date to check. timestring 'Y-m-d'
 * @param      $rangelow lower end of daterange.timestring'Y-m-d'
 * @param      hrangehigh higher end of daterange.timestring of form 'Y-m-d'
 * @return     TRUE if in range or FALSE if not within range
 */
function _is_date_in_range($datetocheck, $rangelow, $rangehigh) {
  $strdatetocheck = date_format(new DateObject($datetocheck), 'Y-m-d');
  $strrangelow = date_format(new DateObject($rangelow), 'Y-m-d');
  $strrangehigh = date_format(new DateObject($rangehigh), 'Y-m-d');
  if ((strtotime($strdatetocheck) >= strtotime($strrangelow)) && (strtotime($strdatetocheck) <= strtotime($strrangehigh))) {
    return TRUE;
  }
  return FALSE;
}

/**
 * get_current_path: current url
 * from drupaluser dhaneshharidas
 * from http://drupal.org/node/46088
 */
function get_current_path() {

  $tmpdest = '';
  $tmpdest = drupal_get_destination();
  //drupal_set_message('<pre>get_current_path'.print_r($tmpdest,true).'</pre>');
  $current_path = explode('=', $tmpdest['destination']);

  // Extracting URL from $current_path
  if (is_array($current_path) && count($current_path) >= 1) {
    if (trim($current_path[0]) != '') {
      $current_url_full = htmlspecialchars(urldecode($current_path[0]));
      // Removing query string
      $current_url_elements = explode('?', $current_url_full);
      if (is_array($current_url_elements)) {
        return trim($current_url_elements[0]);
      }
      return trim($current_url_elements);
    }
    return $_REQUEST['q'];
  }
}

/**
 * Create an array of dates from two string dates.
 *
 * Takes two dates formatted as Y-m-d and creates an inclusive array of the
 * dates between the from and to dates. Could test validity of dates here but
 * I'm already doing that in the main script.
 *
 * @author   Mike Boone
 * @param string $date_from Y-m-d
 * @param string $date_to Y-m-d
 * @return array An inclusive array of the dates between the from and to dates.
 * @see http://boonedocks.net/mike/archives/137-Creating-a-Date-Range-Array-with-PHP.html
 */
function _agreservations_create_date_range($date_from, $date_to) {
  $range = array();
  $temp_date_from = mktime(1, 0, 0, intval(substr($date_from, 5, 2)), intval(drupal_substr($date_from, 8, 2)), intval(substr($date_from, 0, 4)));
  $temp_date_to = mktime(1, 0, 0, intval(substr($date_to, 5, 2)), intval(substr($date_to, 8, 2)), intval(substr($date_to, 0, 4)));  
//  $temp_date_from = mktime(1, 0, 0, drupal_substr($date_from, 5, 2), drupal_substr($date_from, 8, 2), drupal_substr($date_from, 0, 4));
//  $temp_date_to = mktime(1, 0, 0, drupal_substr($date_to, 5, 2), drupal_substr($date_to, 8, 2), drupal_substr($date_to, 0, 4));

  if ($temp_date_to >= $temp_date_from) {
    // first entry
    array_push($range, date('Y-m-d', $temp_date_from));

    while ($temp_date_from < $temp_date_to) {
      $temp_date_from = strtotime("+1 day", $temp_date_from);
      array_push($range, date('Y-m-d', $temp_date_from));
    }
  }
  return $range;
}
function _agreservations_create_datehour_range($date_from, $date_to) {
  $range = array();
  $temp_date_from = mktime(intval(substr($date_from, 11, 2)), 0, 0, intval(substr($date_from, 5, 2)), intval(drupal_substr($date_from, 8, 2)), intval(substr($date_from, 0, 4)));
  $temp_date_to = mktime(intval(substr($date_to, 11, 2)), 0, 0, intval(substr($date_to, 5, 2)), intval(substr($date_to, 8, 2)), intval(substr($date_to, 0, 4)));  
//  $temp_date_from = mktime(1, 0, 0, drupal_substr($date_from, 5, 2), drupal_substr($date_from, 8, 2), drupal_substr($date_from, 0, 4));
//  $temp_date_to = mktime(1, 0, 0, drupal_substr($date_to, 5, 2), drupal_substr($date_to, 8, 2), drupal_substr($date_to, 0, 4));
  
  if ($temp_date_to >= $temp_date_from) {
    // first entry
    array_push($range, date('Y-m-d H:i', $temp_date_from));
//   drupal_set_message('<pre>eeeeeeeehhheeeee ' . print_r($range, true) . '</pre>');
    while ($temp_date_from < $temp_date_to) {
      $temp_date_from = strtotime("+1 hour", $temp_date_from);
      array_push($range, date('Y-m-d H:i', $temp_date_from));
    }
  }
  
  return $range;
}