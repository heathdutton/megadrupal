<?php

class TemplateMustacheView {
  protected $data;

  public function __construct($data) {
    $this->data = $data;
  }

  public function __get($key) {
    if (substr($key, -1) == '?') {
      return !empty($this->data[$key]);
    }
    return $this->data[$key];
  }

  public function __isset($key) {
    if (substr($key, -1) == '?') {
      $key = substr($key, 0, -1);
    }
    return isset($this->data[$key]);
  }

  public function debug() {
    return '<pre>'. var_export($this->data, TRUE) .'</pre>';
  }
}
