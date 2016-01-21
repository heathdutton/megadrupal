<?php


class injapi_InjectedAPI_hookSchema extends injapi_InjectedAPI_Abstract {

  function table($name, $info = array()) {
    $this->data[$name] = $info + array(
      'fields' => array(),
      'module' => $this->module,
    );
    return new injapi_InjectedAPI_hookSchema_Table($name, $this->data[$name]);
  }
}
