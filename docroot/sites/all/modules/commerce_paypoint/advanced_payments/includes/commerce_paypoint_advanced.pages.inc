<?php
/**
 * @file
 * Provides page callbacks.
 */


/**
 * Outputs a temporary page before the PayPoint form is submitted.
 *
 * Displayed when there is a load delay or if the user has Javascript disabled.
 */
function commerce_paypoint_advanced_waiting_page() {
  print ('<html><head><title></title></head><body><p>');
  print t('Please wait to be redirected to the payment page.');
  print ('</p></body></html>');
}


/**
 * Outputs a temporary page after PayPoint payment.
 *
 * Displayed when used with iframe.
 */
function commerce_paypoint_advanced_payment_result_page() {
  print ('<html><head><title>PayPoint payment completed.</title></head><body><p>');
  print t('Payment completed.');
  print ('</p></body></html>');
}
