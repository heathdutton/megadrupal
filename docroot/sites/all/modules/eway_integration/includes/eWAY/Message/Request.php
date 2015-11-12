<?php

namespace eWAY\Message;

use eWAY\Payment\PaymentFactory;

class Request implements RequestInterface {
  private $sandbox;
  private $method;
  private $base;
  private $path;
  private $payment;
  private $configs;

  public function __construct($method, $payment, $configs = array()) {
    $this->method = strtoupper($method);
    $this->sandbox = isset($configs['sandbox']) ? (bool) $configs['sandbox'] : FALSE;
    $this->setConfigs($configs);
    $this->base = $this->getBase();
    $pay = PaymentFactory::createPayment($payment, $this->configs);
    $this->payment = $pay->getPayment();
    $this->path = $pay->getPath();
  }

  /**
   * @return string
   */
  public function getMethod() {
    return $this->method;
  }

  /**
   * @param $method
   * @return $this
   */
  public function setMethod($method) {
    $this->method = strtoupper($method);
    return $this;
  }

  /**
   * @param bool $sandbox
   * @return $this
   */
  public function setSandbox($sandbox = FALSE) {
    $this->sandbox = (bool) $sandbox;
    return $this;
  }

  /**
   * @return bool
   */
  public function getSandbox() {
    return isset($this->sandbox) ? (bool) $this->sandbox : FALSE;
  }

  /**
   * @param $configs
   * @return array
   */
  public function setConfigs($configs) {
    $default = array(
      'sandbox' => FALSE,
    );
    $this->configs = array_merge($default, $configs);
    return $this;
  }

  /**
   * @return array
   */
  public function getConfigs() {
    return is_array($this->configs) ? $this->configs : array();
  }

  /**
   * @param $base
   * @return $this
   */
  public function setBase($base) {
    $this->base = $base;
    return $this;
  }

  public function getBase() {
    return isset($this->base) ? $this->base : $this->setDefaultBase();
  }

  /**
   * @return string
   */
  private function setDefaultBase() {
    $sandbox = $this->getSandbox();
    if ($sandbox === TRUE) {
      $this->base = 'https://api.sandbox.ewaypayments.com';
    }
    else {
      $this->base = 'https://api.ewaypayments.com';
    }
    return $this->base;
  }

  public function getPath() {
    return isset($this->path) ? $this->path : '/';
  }

  public function getPayment() {
    return isset($this->payment) ? $this->payment :NULL;
  }
}
