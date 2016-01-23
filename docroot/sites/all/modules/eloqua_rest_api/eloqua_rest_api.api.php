<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup eloqua_rest_api Eloqua REST API module integrations.
 *
 * Module integrations with the Eloqua REST API module.
 */

/**
 * @defgroup eloqua_rest_api_hooks Eloqua REST API's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend the Eloqua
 * REST API module.
 */


/**
 * Alter the Elomentary client before it's returned for use.
 *
 * This hook is invoked just before the Elomentary client is returned, after
 * initial instantiation and authentication.
 *
 * Use this to further configure the Client, or swap in your own client.
 *
 * @param Eloqua\Client $client
 *   The Elomentary client, already configured with the desired API version and
 *   authenticated.
 *
 * @see eloqua_rest_api_client()
 */
function hook_eloqua_rest_api_client_alter(&$client) {
  // Alter the user-agent string sent by Elomentary to Eloqua.
  $client->getHttpClient()->setOption('user_agent', 'My new User Agent');
}


/**
 * @}
 */
