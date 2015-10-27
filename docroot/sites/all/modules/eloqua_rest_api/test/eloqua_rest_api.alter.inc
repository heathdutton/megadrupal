<?php

/**
 * @file
 * Used to ensure that the Elomentary client is alterable.
 */

/**
 * Implements hook_eloqua_rest_api_client_alter().
 *
 * On behalf of the system module.
 */
function system_eloqua_rest_api_client_alter(&$client) {
  $client->setOption('altered', TRUE);
}
