<?php


class injapi_InjectedAPI_hookMenu extends injapi_InjectedAPI_Abstract {

  function item($path, $info = array()) {
    $this->data[$path] = $info;
    return new injapi_InjectedAPI_hookMenu_Item($path, $this->data[$path]);
  }

  function localTask($path) {
    return $this->item($path, array(
      'type' => MENU_LOCAL_TASK,
    ));
  }

  function defaultLocalTask($path) {
    return $this->item($path, array(
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'title' => 'View',
    ));
  }
}
