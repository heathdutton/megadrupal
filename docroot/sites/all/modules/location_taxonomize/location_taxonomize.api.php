<?php

/**
 * @file
 * API documentation for the Location Taxonomize module.
 */
 
/**
* @addtogroup hooks
* @{
*/

/**
 * Allows to register a module as a source module for Location Taxonomize.
 *
 * @return
 *  Array of source modules, like [module machine name] => [name to appear in UI].
 */
function hook_location_taxonomize_source() {
  // Add the module called 'location_taxonomize_af' as a source module.
  return array(
   'location_taxonomize_af' => 'Address Field', 
  );
}

/**
 * Act during the Location Taxonomize initialization process.
 *
 * This is called during location_taxonomize_initialize, which is called after
 * the initial initialization form is submitted. Use this to take any action
 * when LT is initialized.
 *
 * @param $settings
 *  Array of initialization settings, including the following keys:
 *    - 'source' - the machine name of the selected source module
 *    - 'method' - the initialization method ('new' or 'existing')
 *    - 'hierarchy' - the hierarchy selected for the vocabulary.
 */
function hook_location_taxonomize_initialize($settings) {
  // Display a special message if the source is Address Field.
  if ($settings['source'] == 'location_taxonomize_af') {
    drupal_set_message(t("NOTE: In order to actually taxonomize data from Address Field fields, you must also edit the individual fields and enable the 'Taxonomize locations from this field' checkbox."), 'warning');
  }
}

/**
* @} End of "addtogroup hooks".
*/
