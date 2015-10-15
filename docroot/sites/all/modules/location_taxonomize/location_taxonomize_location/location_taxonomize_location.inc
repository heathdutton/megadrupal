<?php

/**
 * @file
 * Some useful functions for Location taxonomize
 */
 
define('LT_LOC_MODULE_ID', 'location_taxonomize_location');
define('LT_LOC_MODULE_NAME', 'Location Taxonomize Location');
 
/**
 * Keeps a list of all the variables maintained by this module, with their
 * default values.
 */
function _location_taxonomize_location_variables() {
  $fields = location_taxonomize_location_get_fields();
  // set field defaults
  foreach ($fields as $key => $value) {
    if ($key != 'country' && $key != 'province' && $key != 'city') {
      $fields[$key] = 0;
    }
  }
  return array(
    'location_taxonomize_location_settings' => array(
      'fields'  => $fields,
    ),
  );
}

/**
 * Returns the Location module fields that can be used for the Location Vocabulary
 * @param $assoc - whether to return an associative array (TRUE) or just an
 *                 indexed array (FALSE)
 * @param $labels - not implemented
 */
function location_taxonomize_location_get_fields($assoc = TRUE, $labels = FALSE) {
  // get names of fields in the location table
  $fields = drupal_schema_fields_sql('location');
  $return = array();
  // remove unsupported fields
  foreach ($fields as $field) {
    /* formatted this way to try to preserve readability as well as accpetable
       line lengths */
    if ($field != 'lid' &&
        $field != 'additional' &&
        $field != 'latitude' &&
        $field != 'longitude' &&
        $field != 'source' &&
        $field != 'is_primary' &&
        $field != 'postal_code'
    ) {
      if ($assoc) $return[$field] = $field;
      else $return[] = $field;
    }
  }
  return array_reverse($return);
}

/**
 * Sets all this module's variables to their default values
 */
function _location_taxonomize_location_set_defaults() {
  $defaults = _location_taxonomize_location_variables();
  foreach ($defaults as $key => $value) {
    if ($value) variable_set($key, $value);
    else variable_del($key);
  }
}

/**
 * Deletes all variables set by this module
 */
function _location_taxonomize_location_del_variables() {
  $vars = _location_taxonomize_location_variables();
  foreach ($vars as $key => $value) {
    variable_del($key);
  }
}

/**
 * Adds the country name and province name fields to a Location
 */
function location_taxonomize_location_fixup($loc) {
  if (isset($loc['country'])) {
    $loc['country_name'] = location_country_name($loc['country']);
    if (isset($loc['province'])) {
      $loc['province_name'] = location_province_name($loc['country'], $loc['province']);
    }
  }
  return $loc;
}

/**
 * Tells Location Taxonomize the names used in this module for the 'Primary
 * Fields' Country, State/Province, City
 */
function location_taxonomize_location_primary_field_names() {
  return array(
    'country',
    'country_name',
    'province',
    'province_name',
    'city',
  );
}