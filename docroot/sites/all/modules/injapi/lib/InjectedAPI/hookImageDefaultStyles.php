<?php


class injapi_InjectedAPI_hookImageDefaultStyles extends injapi_InjectedAPI_Abstract {

  function style($key, $info = array()) {
    $this->data[$key] = $info + array(
      'effects' => array(),
      'module' => $this->module,
    );
    return new injapi_InjectedAPI_hookImageDefaultStyles_Style($key, $this->data[$key]);
  }
}
