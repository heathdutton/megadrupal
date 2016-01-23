<?php

/**
 * Contains class PayPalPaymentIPN.
 */

/**
 * A PayPal IPN.
 *
 * @see PayPalPaymentIPNController
 * @see paypal_payment_schema()
 */
class PayPalPaymentIPN {

  /**
   * The PID of the Payment this IPN is for.
   *
   * @see PayPalPaymentIPNController::invoiceID()
   * @see PayPalPaymentIPNController::PID()
   *
   * @var string
   */
  public $pid = 0;

  /**
   * The PayPal transaction ID.
   *
   * @var string
   */
  public $txn_id = '';

  /**
   * Constructor.
   */
  function __construct(array $properties = array()) {
    foreach ($properties as $property => $value) {
      $this->$property = $value;
    }
  }
}