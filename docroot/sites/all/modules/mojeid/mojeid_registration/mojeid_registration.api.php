<?php

/**
 * @file
 * This is mojeID registration API documentation.
 */

/**
 * Change the mojeID registration data before they are sent to mojeID.
 *
 * @param $request
 *   Array of required and optional data we are sending to mojeID server.
 */
function hook_mojeid_registration_request_alter(&$data) {
  $data['realm'] = 'https://my-realm.com/';
}

/**
 * Process mojeID registration request before it is sent.
 *
 * @param $request
 *   Array of required and optional data we are sending to mojeID server.
 */
function hook_mojeid_registration_request($data) {
  db_insert('mojeid_registration_log')
    ->fields(array(
      'mojeid' => 'Unknown',
      'status' => 'NEW',
      'nonce' => $data['registration_nonce'],
      'time' => REQUEST_TIME,
    ))
    ->execute();
}

/**
 * Process mojeID registration response.
 *
 * mojeID sends information about user registration status after registration
 * and after each security level validation.
 *
 * @param $request
 *   Array of required and optional data we are sending to mojeID server.
 */
function hook_mojeid_registration_response($data) {
  db_insert('mojeid_registration_log')
    ->fields(array(
      'mojeid' => $data['claimed_id'],
      'status' => $data['status'],
      'nonce' => $data['registration_nonce'],
      'time' => REQUEST_TIME,
    ))
    ->execute();
}
