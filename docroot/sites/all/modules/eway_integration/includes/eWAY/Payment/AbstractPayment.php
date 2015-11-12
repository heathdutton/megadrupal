<?php

namespace eWAY\Payment;

abstract class AbstractPayment {
  private $customer;
  private $shipping;
  private $items;
  private $pay;
  private $options;

  public function getCustomer() {
    return isset($this->customer) ? $this->customer : NULL;
  }

  public function setCustomer($customer) {
    $this->customer = $customer;
    return $this;
  }

  public function getShippingAddress() {
    return isset($this->shipping) ? $this->shipping : NULL;
  }

  public function setShippingAddress($shipping) {
    $this->shipping = $shipping;
    return $this;
  }

  public function getItems() {
    return isset($this->items) ? $this->items : NULL;
  }

  public function setItems($items) {
    $this->items = $items;
    return $this;
  }

  public function getPay() {
    return isset($this->pay) ? $this->pay : NULL;
  }

  public function setPay($pay) {
    $this->pay = $pay;
    return $this;
  }

  public function getOptions() {
    return isset($this->options) ? $this->options : NULL;
  }

  public function setOptions($options) {
    $this->options = $options;
    return $this;
  }

  public function getOption($key) {
    return isset($this->options[$key]) ? $this->options[$key] : NULL;
  }

  public function setOption($key, $value) {
    $this->options[$key] = $value;
    return $this;
  }
}
