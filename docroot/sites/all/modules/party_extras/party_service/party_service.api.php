<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup party_service_hooks Party Service Hooks.
 * @{
 * Hooks that can be implemented by other modules to react to things that
 * happen on a party service.
 */

/**
 * Act when a POST request is received.
 *
 * @param PartyService $service
 *   The service that has received the POST request.
 * @param Party $party
 *   The party that has been created for the POST request.
 * @param stdClass $input
 *   The original unserialized, unprocessed input received.
 */
function hook_party_service_post($service, $party, $input) {
  // Assign the attorney hat to all Party's received through the service.
  if ($service->name == 'test_service') {
    party_hat_hats_assign($party, array('attorney'), TRUE);
  }
}

/**
 * @}
 */
