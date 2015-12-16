<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:15
 */

class ExporterConfig {

  private $name;
  private $prefix;

  function __construct($name) {
    $this->name = $name;
    $this->prefix = 'exporter_' . $name;
  }

  function set($name, $value) {
    variable_set($this->prefix . '_' . $name, $value);
  }

  function get($name, $default_value = NULL) {
    return variable_get($this->prefix . '_' . $name, $default_value);
  }

}
