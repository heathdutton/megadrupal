<?php
/**
 * @file
 * Helper functions to notify about Tealium tags and and retrieve the tag cloud.
 */

/**
 * Function that will be used to set Tealium Tags when they are fired.
 *
 * @param string $tealium_tag_key
 *   Tealium tag key. Ex: s.events
 *
 * @param string $tealium_tag_value
 *   Tealium Tag value.
 *
 * @param bool $merge
 *   Merge Value if exist.
 *
 * @param bool $is_session_based
 *   Is it session based?
 *
 *   Set a flag so that will be returned on the tag cloud through out the
 *   session. Default will be boolean FALSE and the tag will return only on a
 *   request.
 */
function tealium_tag_notify($tealium_tag_key, $tealium_tag_value, $merge = FALSE, $is_session_based = FALSE) {
  // This is for the modules that sets json encoded strings on panels.
  if (is_array($tealium_tag_value) == FALSE) {
    $tealium_tag_value = (_tealium_is_json($tealium_tag_value)) ? json_decode($tealium_tag_value, TRUE) : trim($tealium_tag_value);
  }

  if (!empty($tealium_tag_key) && !empty($tealium_tag_value)) {
    try {
      tealium_set_tag($tealium_tag_key, $tealium_tag_value);
    }
    catch (Exception $e) {
      watchdog('tealium', $e->getMessage(), array(), WATCHDOG_ERROR);
    }
  }
}

/**
 * Simple function to check whether a given string is JSON encoded.
 *
 * @param string $string
 *   String to check.
 *
 * @return bool
 *   Return whether it is a JSON endoded object or not.
 */
function _tealium_is_json($string) {
  json_decode($string);
  return (json_last_error() == JSON_ERROR_NONE);
}

/**
 * Return if its a valid tealium value for a tag.
 * @todo: Move this into the Tealium Object during the refactoring.
 *
 * It should just accept non empty strings or integers.
 *
 * @param string $value
 *   Value to test if it's a valid one.
 *
 * @return bool
 *   TRUE if it's a valid value, FALSE otherwise.
 */
function tealium_is_valid_value($value) {
  // @see TeaiumModuleTest for valid values.
  return $value !== '' && (is_int($value) || is_string($value));
}
