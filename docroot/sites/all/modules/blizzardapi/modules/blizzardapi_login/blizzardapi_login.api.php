<?php

/**
 * @file
 * Documentation and code examples for API developers.
 */

/**
 * Providing custom OAuth 2.0 clients.
 *
 * This module provides an abstraction layer that allows developers to provide
 * their own compatible OAuth 2.0 implementation. Clients should extend the
 * BlizzardApiLoginClient class and provide a concrete implementation of
 * BlizzardApiLoginClientInterface.
 *
 * The client used by this module is defined in the 'blizzardapi_login_client'
 * variable.
 *
 * @see http://tools.ietf.org/html/rfc6749
 */

/**
 * Accessing protected Battle.net API resources.
 *
 * Depending on the OAuth 2.0 client in use, methods for accessing these APIs
 * will vary. Always check the type of the login client before use.
 */
function blizzardapi_login_example() {
  global $user;
  $data = array();
  
  // This does not have to be the region that the user used to login.
  $region = BlizzardApiLoginClient::REGION_US;
  $client = blizzardapi_login_client($region);

  // Example 1: Blizzard API Default Client
  if ($client instanceof BlizzardApiLoginDefaultClient) {
    $token = blizzardapi_login_user_token($user);
    if (empty($token)) {
      return FALSE;
    }
    try {
      // Create a request for the desired information.
      $request = BlizzardApiRequestFactory::getWowProfile($region);
      $data = $client->setAccessToken($token)->sendRequest($request);
    }
    catch (Exception $e) {
      watchdog_exception('blizzardapi_login', $e);
    }
  }

  // Example 2: Battle.net for Google API Clients
  if ($client instanceof BlizzardApiLoginGoogleClient) {
    $service = $client->getServiceForUser($user);
    if (!isset($service)) {
      return FALSE;
    }
    try {
      // See third-party documentation for other API resources.
      $data = $service->profile->getWarcraftCharacters();
    }
    catch (Exception $e) {
      watchdog_exception('blizzardapi_login', $e);
    }
  }
  
  return empty($data) ? FALSE : $data;
}

/**
 * Request additional scopes during the login process.
 *
 * Scopes are essentially permissions for restricted information. Modules
 * should only request the scopes required for their usage. If a scope has not
 * been allowed or denied in the past, the user will receive a prompt. If an
 * invalid scope is provided, the login process will fail.
 *
 * Available scopes (at this time):
 * - sc2.profile: Access StarCraft 2 profile information.
 * - wow.profile: Access a list of all World of Warcraft characters.
 *
 * @return array
 *   A list of scopes.
 */
function hook_blizzardapi_login_scopes() {
  return array('wow.profile', 'sc2.profile');
}

/**
 * Notification when a Battle.net user has logged in.
 *
 * @param object $account
 *   The user object.
 * @param string $region
 *   The Battle.net region used to login.
 *
 * @see blizzardapi_login_is_battlenet_account()
 */
function hook_blizzardapi_login_success($account, $region) {
  // Store the user's login region, as it is likely their preferred region.
  db_merge('example')
    ->key(array('uid' => $account->uid))
    ->fields(array('region' => $region))
    ->execute();
}
