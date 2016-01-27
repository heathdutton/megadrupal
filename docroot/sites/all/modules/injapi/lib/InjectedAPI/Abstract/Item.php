<?php


class injapi_InjectedAPI_Abstract_Item {

  protected $key;
  protected $data;

  function __construct($key, &$data) {
    $this->key = $key;
    $this->data =& $data;
  }

  protected function setOrUnset($key, $value = NULL) {
    if (isset($value)) {
      $this->data[$key] = $value;
    }
    else {
      unset($this->data[$key]);
    }
  }
}
