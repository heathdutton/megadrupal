<?php

namespace eWAY\Payment;

interface PaymentInterface {
  public function getPath();
  public function setPath($path);
  public function getPayment();
  public function setPayment($payment);
}
