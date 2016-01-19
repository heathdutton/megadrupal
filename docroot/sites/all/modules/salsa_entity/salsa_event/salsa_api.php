<?php
/**
 * @file
 * API documentation for salsa_event.module.
 */

/**
 * Alter the display of salsa event fees.
 */
function hook_salsa_event_fee_amount_alter($fee_amount) {
  return t('@amount @currency', array('@amount' => $fee_amount, '@currency' => '$'));
}
