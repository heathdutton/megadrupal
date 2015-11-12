<?php
/**
 * @file
 *
 * Hooks exposed by the vinfield module for lookup providers and external
 * modules to allow interaction with the data managed by this module.
 */

/**
 * hook_vinfield_provider_info
 *
 * Returns a hook_menu-esque subset array of provider info that adds provider global
 * settings under the master VIN Field admin page, keyed by provider machine name.
 * Possible array items are:
 *
 * title            Display name for provider
 * description      Description of provider
 * page callback    Page callback for settings form
 * page arguments   Page callback arguments for settings form
 * query callback   VIN lookup callback; see below
 * always invoke    Set to TRUE to always invoke this provider; use with care
 *                  on paid providers!
 */
function hook_vinfield_provider_info() {
  $items = array();

  $items['sample_provider'] = array(
      'title' => 'Sample validation/lookup provider',
      'description' => 'Description of sample validation/lookup provider',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('sample_provider_settings'),
  );

  return $items;
}

/**
 * hook_vinfield_provider_info_alter
 *
 * Provides an alter hook for all providers collected from invoking hook_vinfield_provider_info
 */
function hook_vinfield_provider_info_alter(&$providers) {
}

/**
 * sample_vinfield_query
 *
 * Callback registered in hook_vinfield_provider_info under 'query callback'
 *
 * Callback to validate or lookup provided VIN with optional params.  Return
 * array is expected to have the following values.
 *
 * year   Model year of vehicle
 * make   Make/manufacturer
 * model  Model of vehicle
 * trim   Trim level, if available
 * extra  Any/all extended data for the VIN
 *
 * If an error occurs, an '#error' element should be added to the array with additional information
 */
function sample_vinfield_query($vin, $params = array()) {
  $result = sample_provider_lookup($vin);
  if ($result) {
    return array(
      'year' => '2001',
      'make' => 'Chevrolet',
      'model' => 'Corvette',
      'trim' => 'Z06'
    );
  }
  else {
    return array(
      '#error' => array(
        'code' => $errno,
        'message' => $message,
      ),
    );
  }
}

/**
 * hook_vinfield_provider_query_alter
 *
 * Provides an alter hook for data retrieved via hook_vinfield_provider_query
 */
function hook_vinfield_provider_query_alter($vin_info) {
}
