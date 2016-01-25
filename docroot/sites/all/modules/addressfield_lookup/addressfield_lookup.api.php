<?php

/**
 * @file
 * API documentation for the Address Field lookup module.
 */

/**
 * Defines an address field lookup service.
 *
 * @return array
 *   An associative array of address field lookup services, keyed by machine
 *   name. The array contains the following values:
 *     - 'name' A human readable name for the service
 *     - 'class' A string specifying the PHP class that implements the
 *     AddressFieldLookupInterface interface.
 *     - 'description' A brief description of the address field lookup service.
 *     - 'config path' The path to the configuration form for this service. The
 *     path will be used as an inline link on the address field lookup module
 *     configuration form. Note that the module implementing the hook must also
 *     define the menu URL and callback.
 *     - 'test data' An example value that will be used to test the status of
 *     connectivity to the service.
 *
 * @see addressfield_lookup_services()
 * @see hook_addressfield_lookup_service_info_alter()
 */
function hook_addressfield_lookup_service_info() {
  return array(
    'my_awesome_postcode' => array(
      'name' => t('My awesome postcode API'),
      'class' => 'MyAwesomePostcodeAPI',
      'description' => t('Provides an address field lookup service based on integration with the My Awesome Postcode API.'),
      'config path' => 'admin/config/regional/addressfield-lookup/my-awesome-addressfield-lookup-service/configure',
      'test data' => 'BH15 1HH',
    ),
  );
}

/**
 * Alters the list of address field lookup services defined by other modules.
 *
 * @param array $addressfield_lookup_services
 *   The array of address field lookup services defined by other modules.
 *
 * @see hook_addressfield_lookup_service_info()
 */
function hook_addressfield_lookup_service_info_alter(array &$addressfield_lookup_services) {
  // Swap in a new REST class for the My Awesome Postcode API.
  $addressfield_lookup_services['my_awesome_postcode']['class'] = 'MyAwesomePostcodeRestAPI';
}

/**
 * Instantiate the address look service class and return the object wrapper.
 *
 * @param string $class
 *   Name of the PHP class that implements AddressFieldLookupInterface.
 *
 * @return AddressFieldLookupInterface
 *   The instantiated class.
 */
function hook_addressfield_lookup_get_service_object($class) {
  // Get the API config.
  $my_awesome_postcode_config = my_awesome_postcode_get_config();

  // Instantiate the API class.
  $my_awesome_postcode_api = new $class($my_awesome_postcode_config['api_key'], $my_awesome_postcode_config['api_secret']);

  return $my_awesome_postcode_api;
}

/**
 * Alter the postcode before the lookup service search is run.
 *
 * @param string $post_code
 *   The postcode from the address field handler.
 */
function hook_addressfield_lookup_postcode_alter(&$post_code) {
  // Strip out spaces and force uppercase.
  $post_code = drupal_strtoupper(preg_replace('/\s/si', '', $post_code));
}

/**
 * Alter the results of an address field lookup search.
 *
 * @param array $results
 *   Array containing the lookup search results.
 */
function hook_addressfield_lookup_results_alter(array &$results) {
  // Add a full address string to each result.
  foreach ($results as $key => $result) {
    $results[$key]['full_address'] = $results[$key]['street'] . ' ' . $results[$key]['place'];
  }
}

/**
 * Update/alter the addressfield format defined by addressfield_lookup.
 *
 * @param array $format
 *   The address format being generated.
 * @param array $address
 *   The address this format is generated for.
 *
 * @return array $format
 *   The address format with any changes made.
 */
function hook_addressfield_lookup_format_update(array $format, array $address) {
  // Re-order the premise element.
  $format['street_block']['premise']['#weight'] = -9;

  return $format;
}
