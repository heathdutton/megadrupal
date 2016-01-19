<?php

/**
 * @file
 * API file for Commerce Zuora.
 */

/**
 * Add or alter the prepopulated fields displayed on the payment iframe.
 *
 * @param array $fields
 * @param $order
 */
function hook_commerce_zuora_hpm_prepopulated_fields(array &$fields, $order) {
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Add phone number added from billing profile.
  $fields['phone'] = $wrapper->commerce_customer_billing->field_phone->value();
}
