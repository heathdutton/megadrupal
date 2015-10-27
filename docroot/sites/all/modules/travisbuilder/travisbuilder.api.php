<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup travisbuilder_hooks Travis Builder hooks.
 *
 * Module integrations with the Travis service API module.
 */


/**
 * Alter the Travis API client before it's returned for use.
 *
 * This hook is invoked just before the Travis client is returned. Use this to further configure
 * the Client, or swap in your own client.
 *
 * @param Travis\Client $client
 *   The Travis client, already configured with API URL picked and authentication token passed.
 *
 * @see travisbuilder_client()
 */
function hook_eloqua_rest_api_client_alter(&$client) {
  // Alter the client.
  $this->browser->addListener(new myListener($token));
}


/**
 * @}
 * End hook documentation.
 */
