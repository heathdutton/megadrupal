<?php

/**
 * @file
 * Defines the Commerce Parcel Monkey alter functions for Drupal Commerce.
 */

/**
 * Allows modules to alter the Parcel Monkey services that are available.
 *
 * @param array $parcel_monkey_services
 *   The array of shipping services that are available.
 *
 * @see hook_commerce_parcel_monkey_services_list_alter()
 */
function hook_commerce_parcel_monkey_services_list_alter(&$parcel_monkey_services) {
  // No example.
}

/**
 * Allows modules to alter the shipping request before it is sent.
 *
 * @param object $rate_request
 *   The request object that gets sent to Parcel Monkey.
 *
 * @see hook_commerce_parcel_monkey_rate_request_alter()
 */
function hook_commerce_parcel_monkey_rate_request_alter(&$rate_request) {
  // No example.
}
