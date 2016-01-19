<?php

/**
 * @file
 * API documentation for the services token module.
 */

/**
 * Return properties used to compute the authentication token.
 *
 * The authentication token is recomputed on every request, and compared to the
 * one sent by the user agent. Hence, the token is invalidated automatically
 * whenever one of the values changes the token is derived from.
 *
 * By default the token is based on the username, password hash and status as
 * well as the authentication realm. As a result, it is enough to change the
 * password in order to invalidate all authentication tokens.
 *
 * @param string $realm
 *   A string defining a group of resources this token is valid for.
 * @param int $uid
 *   The user id the token should be generated for.
 */
function hook_services_token_properties($realm, $uid) {
  // Ensures that the API key changes whenever roles/permissions are modified.
  $account = user_load($uid);
  $user_permissions = call_user_func_array('array_merge', user_role_permissions($account->roles));
  return array(
    'permissions' => $user_permissions,
  );
}

/**
 * Modify properties used to compute the authentication token.
 *
 * @param array $properties
 *   The properties used to cumpute the authentication token.
 * @param string $realm
 *   A string defining a group of resources this token is valid for.
 * @param int $uid
 *   The user id the token should be generated for.
 */
function hook_services_token_properties_alter(&$properties, $realm, $uid) {
  // No example.
}

/**
 * Modify the expiry date upon token generation.
 *
 * @param int $expires
 *   The Unix timestamp of the time when the token will expire.
 * @param string $realm
 *   A string defining a group of resources this token is valid for.
 * @param int $uid
 *   The user id the token should be generated for.
 */
function hook_services_token_expires_alter(&$expires, $realm, $uid) {
  // No example.
}

/**
 * Modify the response sent to the client.
 *
 * @param object $response
 *   The response data as produced by services_token_create().
 * @param string $realm
 *   A string defining a group of resources this token is valid for.
 * @param int $uid
 *   The user id the token was generated for.
 *
 * @see services_token_create().
 */
function hook_services_token_create_alter($response, $realm, $uid) {
  // No example.
}
