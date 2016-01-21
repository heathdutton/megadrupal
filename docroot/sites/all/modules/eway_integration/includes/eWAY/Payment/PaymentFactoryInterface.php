<?php

namespace eWAY\Payment;

interface PaymentFactoryInterface {
  static function createPayment($payment, $configs);
}
