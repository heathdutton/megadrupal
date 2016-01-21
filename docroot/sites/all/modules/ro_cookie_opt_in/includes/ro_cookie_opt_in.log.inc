<?php

/**
 * @file
 * Rijksoverheid Cookie Opt-in log functions.
 */

/**
 * Log page callback function.
 */
function ro_cookie_opt_in_log() {
  // Check if we need Drupal to log the response.
  if (variable_get('ro_cookie_opt_in_log enabled', TRUE)) {
    // Retrieve the response data from the request.
    $query = drupal_get_query_parameters();
    $combi_response = $query[check_plain(variable_get('ro_cookie_opt_in_cookie_name', 'consent_for_cookies'))];
    $response_data_array = explode('.', $combi_response);

    if (!empty($response_data_array)) {
      $response = $response_data_array[0];
      $unique_id = NULL;
      if (isset($response_data_array[1])) {
        $unique_id = $response_data_array[1];
      }

      // Write the response data to the log.
      $query = db_insert('ro_cookie_opt_in_log')
        ->fields(array(
          'timestamp' => time(),
          'response' => $response,
          'unique_id' => $unique_id,
        ))
        ->execute();
    }
  }

  return '';
}
