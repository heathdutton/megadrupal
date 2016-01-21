<?php
/**
 * @file
 * Document hooks.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Handle incoming request from Google Checkout.
 *
 * An acknowledgement response will automatically be sent once the hook has
 * completed.
 *
 * @param $type
 *   The request type.
 * @param $root
 *   The root XML element as a SimpleXML object.
 * @param $serial_number
 *   The serial number of the request.
 */
function hook_google_checkout_request($type, $root, $serial_number) {
  if ($type == 'authorization-amount-notification') {
    $order_number = (string) $root->{'google-order-number'};
    $amount = (int) $root->{'authorization-amount'};

    // Store it or do something cool.
  }
}

/**
 * @} End of "addtogroup hooks".
 */
