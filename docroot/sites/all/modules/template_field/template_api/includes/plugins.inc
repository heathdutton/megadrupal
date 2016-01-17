<?php

abstract class TemplateApiPlugin {
  // The plugin definition from ctools
  protected $plugin_definition;

  /**
   * Contractor
   *
   * @param $plugin_definition
   * @param $key
   * @param $definition
   */
  public function __construct($plugin_definition) {
    $this->plugin_definition = $plugin_definition;
  }
}

abstract class TemplateApiPluginController {
  protected $plugin;
  protected $plugin_definition;

  public function __construct($plugin_key) {
    ctools_include('plugins');
    if ($plugin_definition = ctools_get_plugins('template_api', $this->pluginType(), $plugin_key)) {
      $this->plugin_definition = $plugin_definition;
      $klass = $plugin_definition['class'];
      if (class_exists($klass)) {
        $this->plugin = new $plugin_definition['class']($plugin_definition);
      }
      else {
        throw new Exception("$klass cannot be found, trying to load $plugin_key plugin.");
      }
    }
  }

  /**
   * The plugin type
   */
  protected function pluginType() {
    return '';
  }

  public function __get($name) {
    return $this->plugin->$name;
  }

  public function getPlugin() {
    return $this->plugin;
  }
}
