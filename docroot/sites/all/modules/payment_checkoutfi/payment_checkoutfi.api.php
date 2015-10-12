<?php

/**
 * @file
 * Hook documentation for the Checkout.fi for Payment module.
 */

/**
 * Alter the payment form data before it is sent to Checkout.fi.
 *
 * @param array &$data
 *   Array of POST field data.
 * @param object $payment
 *   The payment object related to this execution.
 */
function hook_payment_checkoutfi_form_data_alter(array &$data, Payment $payment) {
  // Add a custom message to payment.
  $data['MESSAGE'] = t('Example store payment');
  // Change delivery date.
  $data['DELIVERY_DATE'] = date('Ymd', strtotime('+1 week'));
  // Add user email.
  $data['EMAIL'] = user_load($payment->uid)->mail;
}

/**
 * Alter the reference number base.
 *
 * @param int &$base
 *   The reference number base.
 * @param object $payment
 *   The payment object related to this execution.
 */
function hook_payment_checkoutfi_form_data_alter(array &$base, Payment $payment) {
  // Change reference number base to format
  // current_year-current_month-payment-id.
  $base = date('Ym') . $payment->pid;
}
