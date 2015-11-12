<?php

/**
 * @file
 * Some useful functions for Location taxonomize
 */
 
define('LT_AF_MODULE_ID', 'location_taxonomize_af');
define('LT_AF_MODULE_NAME', 'Location Taxonomize Address Field');
 

/**
 * Keeps a list of all the variables maintained by this module, with their
 * default values.
 *
 * @param $fields
 *  A boolean indicating whether to actually load the hierarchy fields variable.
 *  This is made FALSE for variable deletion, for example, when the actual fields
 *  subarray is not important and does not need to be calculated.
 */
function _location_taxonomize_af_variables($include_fields = TRUE) {
  if ($include_fields) {
    $fields = location_taxonomize_af_get_fields(TRUE);
  }
  else {
    $fields = array();
  }
  // set field defaults
  foreach ($fields as $key => $value) {
    if ($key != 'country' && $key != 'administrative_area' && $key != 'locality') {
      $fields[$key] = 0;
    }
  }
  return array(
    'location_taxonomize_af_settings' => array(
      'fields'  => $fields,
    ),
  );
}

/**
 * Returns the Address Field fields that can be used for the Location Vocabulary
 * @param $assoc - whether to return an associative array (TRUE) or just an
 *                 indexed array (FALSE)
 * @param $labels - whether to return the labels of fields as array values
 */
function location_taxonomize_af_get_fields($assoc = TRUE, $labels = FALSE) {
  // get fields from Address Field
  $fields = addressfield_data_property_info();
  foreach($fields as $key => $value) {
    $fields[$key] = $value['label'];
  }
  // define the fields we support and their order
  $supported = array(
    'country',
    'administrative_area',
    'sub_administrative_area',
    'locality',
    'dependent_locality',
    'postal_code',
    'organisation_name',
    'thoroughfare',
    'premise',
  );
  $return = array();
  // remove unsupported fields
  foreach ($supported as $field) {
    if ($assoc) {
      if ($labels) $return[$field] = $fields[$field];
      else $return[$field] = $field;
    }
    else $return[] = $field;
  }
  return $return;
}

/**
 * Sets all this module's variables to their default values
 */
function _location_taxonomize_af_set_defaults() {
  $defaults = _location_taxonomize_af_variables();
  foreach ($defaults as $key => $value) {
    if ($value) variable_set($key, $value);
    else variable_del($key);
  }
}

/**
 * Deletes all variables set by this module
 */
function _location_taxonomize_af_del_variables() {
  $vars = _location_taxonomize_af_variables(FALSE);
  foreach ($vars as $key => $value) {
    variable_del($key);
  }
}

/**
 * Tells Location Taxonomize the names used in this module for the 'Primary
 * Fields' defined by Location Taxonomize
 */
function location_taxonomize_af_primary_field_names() {
  return array(
    'country',
    'country_name',
    'administrative_area',
    'administrative_area_name',
    'locality',
  );
}