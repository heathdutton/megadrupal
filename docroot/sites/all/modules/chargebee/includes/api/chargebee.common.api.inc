<?php
/**
 * @file
 * Common ChargeBee API functions.
 */

/**
 * Load library and setup ChargeBee environment.
 *
 * @return bool
 *   Setup environment status.
 */
function _chargebee_common_init() {
  $library = libraries_load('chargebee-php');
  if (!$library['loaded']) {
    return FALSE;
  }
  $api_site_name = variable_get('chargebee_api_site_name');
  $api_key = variable_get('chargebee_api_key');
  if (!$api_site_name || !$api_key) {
    return FALSE;
  }
  ChargeBee_Environment::configure($api_site_name, $api_key);
  return TRUE;
}

/**
 * Convert object to array by list attributes.
 *
 * @param object $object
 *   Object for convert.
 * @param array $attributes
 *   Array and object attributes.
 *
 * @return array
 *   Converted array.
 */
function _chargebee_object_to_array_convert($object, $attributes) {
  $array = array();
  foreach ($attributes as $array_attribute => $object_attribute) {
    $array[$array_attribute] = isset($object->$object_attribute) ? $object->$object_attribute : NULL;
  }
  return $array;
}
