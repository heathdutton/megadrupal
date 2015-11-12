<?php

namespace eWAY\Payment;

class Payment extends AbstractPayment implements PaymentInterface {
  private $data;
  private $path;

  /**
   * @param $payment
   * @param array $configs
   */
  public function __construct($payment, $configs = array()) {
    $this->data = new Collection();
    $this->createPath($configs);
    $this->setPayment($payment);
  }

  /**
   * @return string
   */
  public function getPath() {
    return '/' . ltrim($this->path, '/');
  }

  /**
   * @param $path
   * @return $this
   */
  public function setPath($path) {
    $this->path = $path;
    return $this;
  }

  /**
   * @return \eWAY\Payment\Collection|null
   */
  public function getPayment() {
    return isset($this->data) ? $this->data : NULL;
  }

  /**
   * @param $payment
   */
  public function setPayment($payment) {
    $this->setCustomer($payment['Customer']);
    $this->setShippingAddress($payment['ShippingAddress']);
    $this->setItems($payment['Items']);
    $this->setPay($payment['Payment']);
    $this->setOptions($payment['Options']);
    $this->data->Customer = $this->getCustomer();
    $this->data->ShippingAddress = $this->getShippingAddress();
    $this->data->Items = $this->getItems();
    $this->data->Payment = $this->getPay();
    $options = $this->getOptions();
    foreach ($options as $k => $v) {
      $this->data->$k = $v;
    }
  }

  /**
   * @param $configs
   */
  protected function createPath($configs) {
    $mode = $configs['method'];
    $path = '/';
    switch ($mode) {
      case 'eway_dc':
        $path .= 'Transaction';
        break;
    }
    $this->setPath($path);
  }
}
