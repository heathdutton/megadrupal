<?php

/**
 * @file
 * Payment hooks.
 */

/**
 * Implements hook_payment_method_controller_info().
 */
function ideal_payment_method_controller_info() {
  return array('iDEALVersion2PaymentMethodController', 'iDEALVersion3PaymentMethodController');
}