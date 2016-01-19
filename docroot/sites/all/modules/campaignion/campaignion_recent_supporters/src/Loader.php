<?php

namespace Drupal\campaignion_recent_supporters;

class Loader {
  protected $backends;

  protected static $instance = NULL;

  public static function instance() {
    if (!self::$instance) {
      self::$instance = new static();
    }
    return self::$instance;
  }

  public function __construct() {
    $hook = 'campaignion_recent_supporters_backend_info';
    $this->backends = \module_invoke_all($hook);
    drupal_alter($hook, $this->backends);
  }

  public function getBackend($name) {
    if (isset($this->backends[$name])) {
      $class = $this->backends[$name];
      return new $class;
    }
  }

  public function backendOptions() {
    $backends = array();
    foreach ($this->backends as $name => $class) {
      $backends[$name] = $class::label();
    }
    return $backends;
  }
}
