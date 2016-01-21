<?php

namespace eWAY\Payment;

class PaymentFactory implements PaymentFactoryInterface {
  /**
   * @param $payment
   * @param $configs
   * @return \eWAY\Payment\Payment
   */
  static function createPayment($payment, $configs) {
    return new Payment($payment, $configs);
  }
}
