<?php

/**
 * @file
 */

/**
 * Modify the price from an external price to an internal price.
 *
 * This hook is used to allow the an external module such change the price
 * based upon some external value such as an exchange rate
 *
 * @param &$price
 *  The value of the existing price which can be altered.
 */
function hook_ec_inbound_price(&$price) {
  $price *= 0.85; // Convert from USD to AUD
}

/**
 * Convert the display of the price to another value
 *
 * This hook is to allow the altering of the amount being displayed based upon
 * some external value such as an exchange rate.
 *
 * @param &$price
 *  The value of the price to be adjusted
 */
function hook_ec_outbound_price(&$price) {
  $price *= 1.15; // Convert from AUD to USD
}
