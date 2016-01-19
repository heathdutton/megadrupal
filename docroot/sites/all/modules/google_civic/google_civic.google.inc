<?php

/**
 * @file Google Civic API Drupal-specific helper functions.
 */

/**
 * Creates a GoogleCivicAPI instance.
 *
 * @param string $api_key
 *   A Google Civic API Key string.
 *
 * @return GoogleCivicAPI
 *   A GoogleCivicAPI instantiated with $api_key.
 */
function google_civic_get_api($api_key = NULL) {
  module_load_include('php', 'google_civic', 'GoogleCivicAPI.class');
  $api_key = $api_key ? $api_key : variable_get('google_civic_key', '');
  return new GoogleCivicAPI($api_key);
}

/**
 * Translates an address response object into an address string.
 *
 * @param Object $address_response
 *   An address object from the Google Civic API.
 *
 * @return string
 *   The address object as an address string.
 */
function google_civic_translate_address_response($address_response) {
  $street_address = google_civic_address_response_street_address(
                                                        $address_response);

  $location_name = !empty($address_response->locationName) ? $address_response->locationName : "";
  $city = !empty($address_response->city) ? $address_response->city : "";
  $state = !empty($address_response->state) ? $address_response->state : "";
  $zip = !empty($address_response->zip) ? $address_response->zip : "";

  if ($location_name && ($street_address || $city || $state || $zip)) {
    $location_name .= ',';
  }

  if ($street_address && ($city || $state || $zip)) {
    $street_address .= ',';
  }

  if ($city && ($state || $zip)) {
    $city .= ',';
  }

  $address_parts = array();
  $address_parts[] = $location_name;
  $address_parts[] = $street_address;
  $address_parts[] = $city;
  $address_parts[] = $state;

  // Don't know why Google returns kind of an odd zip code, but we want to
  // fix it.
  if (strlen($zip) == 9) {
    $zip = substr($zip, 0, 5) . "-" . substr($zip, 5, 9);
  }
  $address_parts[] = $zip;

  $address = join(' ', $address_parts);
  return $address;
}

/**
 * Extracts a street address from a Google Civic address response.
 *
 * @param Object $address_response
 *   A Google Civic address response.
 *
 * @return string
 *   A street address string.
 */
function google_civic_address_response_street_address($address_response) {
  $street_address_parts = array();
  $street_address_parts[] = $address_response->line1;
  $street_address_parts[] = !empty($address_response->line2) ? $address_response->line2 : "";
  $street_address_parts[] = !empty($address_response->line3) ? $address_response->line3 : "";
  $street_address = trim(join(' ', $street_address_parts));
  return $street_address;
}
