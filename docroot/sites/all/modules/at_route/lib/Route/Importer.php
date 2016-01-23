<?php
namespace Drupal\at_route\Route;

class Importer {
  /**
   * @var string
   */
  private $module;

  /**
   * @var string
   */
  private $path;

  public function setModule($module) {
    $this->module = $module;
  }

  public function getConfigPath() {
    $this->path = DRUPAL_ROOT . '/' . drupal_get_path('module', $this->module) . '/config/route.yml';
    return file_exists($this->path) ? $this->path : FALSE;
  }

  public function import() {
    if (!$data = at_config_read_yml($this->path)) return array();
    if (empty($data['routes'])) return array();

    foreach ($data['routes'] as $route_name => $route_data) {
      if ($item = at_id(new RouteToMenu($this->module, $route_name, $route_data))->convert()) {
        $items[$route_name] = $item;
      }
    }

    return !empty($items) ? $items : array();
  }
}
