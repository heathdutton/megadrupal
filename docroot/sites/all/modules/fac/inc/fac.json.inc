<?php

/**
 * @file
 * This file contains the json callback of the Fast Autocomplete module.
 */

/**
 * Fast AutoComplete json callback.
 */
function fac_generate_json($language, $key) {
  // Remove the .json part from the key.
  $key = str_replace('.json', '', $key);

  // Only continue if the requested key is shorter or equal to the configured
  // key length to search for.
  if (strlen($key) <= variable_get('fac_key_max_length', 5)) {
    // Get the result for the key which automatically saves a json file on the
    // file system.
    $json_repsonse = fac_generate_json_for_key($language, $key);

    // Return the json result.
    drupal_add_http_header('Content-Type', 'application/json');
    echo $json_repsonse;
  }

  drupal_exit();
}
