<?php

namespace Drupal\go1_base\Route;

class RouteToMenu {
  /**
   * @var string
   */
  private $module;

  /**
   * @var string
   */
  private $route_name;

  /**
   * @var array
   */
  private $route_data;

  /**
   * @var array
   */
  private $menu_item = array();

  /**
   * @param string $module
   * @param string $route_name
   * @param array $route_data
   */
  public function __construct($module, $route_name, $route_data) {
    $this->module     = $module;
    $this->route_name = $route_name;
    $this->route_data = $route_data;
  }

  /**
   * We can not define constants in yaml, this method is to convert them.
   */
  public function convert(&$error = '') {
    $this->menu_item = $this->route_data + array(
      'pattern' => $this->route_name,
      'file path' => drupal_get_path('module', $this->module)
    );

    // Parse constants
    $this->parseConstants();

    if ($this->validate($error)) {
      if (!empty($this->menu_item['page callback'])) {
        $this->menu_item['function'] = $this->menu_item['page callback'];
      }

      $this->menu_item['page callback'] = 'Drupal\go1_base\Route\Controller::pageCallback';
      $this->menu_item['page arguments'] = isset($this->menu_item['page arguments']) ? $this->menu_item['page arguments'] : array();
      array_unshift($this->menu_item['page arguments'], $this->menu_item);

      return $this->menu_item;
    }

    return FALSE;
  }

  /**
   * @todo Remove this in flavor it typed-data
   */
  private function parseConstants() {
    $el = go1_container('expression_language');

    if (!empty($this->menu_item['context'])) {
      $this->menu_item['context'] = $el->evaluate($this->menu_item['context']);
    }

    if (!empty($this->menu_item['type'])) {
      $this->menu_item['type'] = $el->evaluate($this->menu_item['type']);
    }

    if (!empty($this->menu_item['cache']['type'])) {
      $this->menu_item['cache']['type'] = $el->evaluate($this->menu_item['cache']['type']);
    }
  }

  private function validate(&$error) {
    return go1_data(
              go1_config('go1_base', 'schema/route')->getAll(),
              $this->menu_item)
            ->validate($error);
  }
}
