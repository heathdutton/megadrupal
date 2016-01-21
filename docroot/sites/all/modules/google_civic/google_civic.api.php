<?php

/**
 * @file API Documentation file
 * This file contains API documentation for the Google Civic module.
 * Note that this code is exemplary; it is never executed when
 * the Google Civic module.
 */

/**
 * Hook to consume the voterinfo response.
 *
 * @param Object $response
 *   A voterinfo response from Google Civic.
 *
 * @see GoogleCivicAPI::request_voterinfo (GoogleCivicAPI.class.php).
 */
function hook_google_civic_voterinfo($response) {
  $voter_postal_code = $response->normalizedInput->zip;
}

