<?php
/**
 * @file
 */

// @todo add an interface.

class SlateModelPlugin {

  protected $plugin;
  protected $model;

  public function __construct($plugin, $model) {
    $this->plugin = $plugin;
    $this->model = $model;
  }

  public static function factory($plugin, $model) {
    ctools_include('plugins');

    $class = ctools_plugin_get_class($plugin, 'handler');

    if (class_exists($class) && is_subclass_of($class, __CLASS__)) {
      return new $class($plugin, $model);
    }

    // @todo throw error!
  }

  public function requiredContexts() {
    return array();
  }

  public function wrapper($model) {
    return $model;
  }

}
