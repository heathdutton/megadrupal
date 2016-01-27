<?php


abstract class injapi_InjectedAPI_Abstract {

  protected $data;
  protected $module;

  function __construct(&$data) {
    $this->data =& $data;
  }

  function setModule($module) {
    $this->module = $module;
  }
}
