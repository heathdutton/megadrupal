<?php

/**
 * @file
 * An interface for doing calls to the signup API proxy.
 */

define('BOTR_SIGNUP_API_URL', 'https://dashboard.bitsontherun.com/signup/');

/**
 * Perform a signup request.
 *
 * Output is similar to the /accounts/credentials/show API call.
 */
function botr_request_signup($email, $password) {
  // Determine which HTTP library to use:
  // check for cURL, else fall back to file_get_contents
  if (function_exists('curl_init')) {
    $library = 'curl';
  }
  else {
    $library = 'fopen';
  }

  $url = BOTR_SIGNUP_API_URL . '?' . http_build_query(array(
    'email' => $email,
    'password' => $password,
  ), "", "&");

  $response = NULL;
  switch ($library) {
    case 'curl':
      $curl = curl_init();
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($curl, CURLOPT_URL, $url);
      $response = curl_exec($curl);
      break;

    default:
      $response = file_get_contents($url);
  }

  $unserialized_response = @json_decode($response, TRUE);

  return $unserialized_response ? $unserialized_response : $response;
}
