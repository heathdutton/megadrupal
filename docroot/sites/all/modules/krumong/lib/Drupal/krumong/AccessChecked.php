<?php

namespace Drupal\krumong;


class AccessChecked {

  protected $wrapped;

  function __construct($wrapped) {
    $this->wrapped = $wrapped;
  }

  function __call($method, $args) {
    if (user_access('access devel information')) {
      return call_user_func_array(array($this->wrapped, $method), $args);
    }
  }
}
