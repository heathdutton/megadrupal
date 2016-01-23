<?php

/**
 * Determines whether the current user can access to a resource.
 *
 * @param string $permission
 *   String indicating which permission to check access for.
 *
 * @return boolean
 *   Boolean indicating whether or not the user has access to the resource.
 *
 * @see user_access()
 */
function _services_addressfield_resource_access($permission) {
  return user_access($permission);
}

/**
 * @see country_get_list()
 */
function services_addressfield_country_get_list() {
  include_once DRUPAL_ROOT . '/includes/locale.inc';
  return country_get_list();
}

/**
 * @see addressfield_get_address_format()
 */
function services_addressfield_get_address_format($country_code) {
  module_load_include('inc', 'addressfield', 'addressfield.address_formats');
  return addressfield_get_address_format($country_code);
}

/**
 * @see addressfield_get_administrative_areas()
 */
function services_addressfield_get_administrative_areas($country_code) {
  module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');
  return addressfield_get_administrative_areas($country_code);
}

/**
 * @see services_addressfield_get_address_format()
 * @see services_addressfield_get_administrative_areas()
 */
function services_addressfield_get_address_format_and_administrative_areas($country_code) {
  $result = new stdClass();
  $result->address_format = services_addressfield_get_address_format($country_code);
  $result->administrative_areas = services_addressfield_get_administrative_areas($country_code);
  return $result;
  /*return = array(
    'address_format' => services_addressfield_get_address_format($country_code),
    'administrative_areas' => services_addressfield_get_administrative_areas($country_code)
  );*/
}

