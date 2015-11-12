<?php

/**
 * @file
 * Hooks provided by commerce_vivapayments.
 */

/**
 * Alter the options of commerce_vivapayments_configuration().
 *
 * This hook is called after setting the default options and let
 * other modules to change the default options.
 *
 * @param array $conf
 *   An array representing the deault configuration.
 */
function hook_commerce_vivapayments_configuration_alter(&$conf) {
  // Change the live server configuration.
  $conf['live_server'] = 'https://www.vivapayments.com/api/orders';
}
