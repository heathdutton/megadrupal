<?php

/**
 * @file
 * Documents hooks provided by the commerce_dps_account_to_account module.
 */

/**
 * Allow other modules to alter the transaction data before sending it to DPS.
 *
 * @param string $reference
 *   The order referende string you would like to use.
 * @param object $order
 *   Commerce order.
 * @param array $settings
 *   commerce_dps configuration settings.
 */
function hook_commerce_dps_account_to_account_merchant_reference_alter($reference, $order, $settings) {
  // No example.
}
