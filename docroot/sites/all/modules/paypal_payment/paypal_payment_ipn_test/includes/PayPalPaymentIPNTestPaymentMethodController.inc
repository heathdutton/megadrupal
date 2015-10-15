<?php

/**
 * Contains PayPalPaymentIPNTestPaymentMethodController.
 */

/**
 * A dummy class that simply implements PayPalPaymentIPNPaymentMethodControllerInterface.
 */
class PayPalPaymentIPNTestPaymentMethodController extends PaymentMethodController implements PayPalPaymentIPNPaymentMethodControllerInterface {

  function __construct() {
    $this->title = __CLASS__;
  }

  /**
   * Implements PayPalPaymentIPNPaymentMethodControllerInterface::PayPalValidateIPNVariables().
   */
  static function PayPalValidateIPNVariables(Payment $payment, array $ipn_variables) {
    return TRUE;
  }

  /**
   * Implements PayPalPaymentIPNPaymentMethodControllerInterface::PayPalProcessIPN().
   */
  static function PayPalProcessIPN(Payment $payment, array $ipn_variables) {
  }
}