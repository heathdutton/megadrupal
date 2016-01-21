<?php

namespace Drupal\manual_direct_debit_uk;

class AccountDataController extends \Drupal\manual_direct_debit\AccountDataController {
  /**
   * Define callbacks and classes.
   */
  public function __construct() {
    parent::__construct();
    $this->title = t('Collect account data (UK)');
    $this->form = new \Drupal\manual_direct_debit_uk\AccountForm();
  }

  function validate(\Payment $payment, \PaymentMethod $payment_method, $strict) {
    parent::validate($payment, $payment_method, $strict);
    $interval = $payment->contextObj->value('donation_interval');
    if (!$interval || $interval == '1') {
      throw new \PaymentValidationException('This payment method does not support one-off payments.');
    }
  }
}
