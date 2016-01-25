<?php

/**
 * @file
 * Hooks provided by the Commerce Sermepa module.
 */

/**
 * Allows modules to configure Sermepa/Redsys gateway to a given order.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param commerce_order $order
 *   The commerce order object.
 */
function hook_commerce_sermepa_gateway($gateway, $order) {
  // No example.
}
