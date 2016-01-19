<?php
namespace Drupal\go1_ui\Controller\Reports;

class Routes {
  public function render() {
    $rows = array();
    foreach (go1_modules('go1_base', 'routes') as $module) {
      foreach (go1_config($module, 'routes')->get('routes') as $path => $route) {
        $attached = array();
        if (isset($route['attached'])) {
          $attached = $route['attached'];
          unset($route['attached']);
        }

        $blocks = array();
        if (isset($route['blocks'])) {
          $blocks = $route['blocks'];
          unset($route['blocks']);
        }

        $breadcrumbs = array();
        if (isset($route['breadcrumbs'])) {
          $breadcrumbs = $route['breadcrumbs'];
          unset($route['breadcrumbs']);
        }
        $breadcrumbs = array_merge($breadcrumbs, $this->findExternalBreadcrumbs($path));

        $rows[] = array($module, $path, go1dr($route), go1dr($attached), go1dr($blocks), go1dr($breadcrumbs));
      }
    }

    return array('#theme' => 'table',
      '#header' => array(
        array('data' => 'Module', 'width' => '100px'),
        array('data' => 'Path', 'width' => '100px'),
        'Route',
        'Attached',
        'Blocks',
        'Breadcrumb',
      ),
      '#rows' => $rows
    );
  }

  public function findExternalBreadcrumbs($path) {
    foreach (go1_modules('go1_base', 'breadcrumb') as $module) {
      $config = go1_config($module, 'breadcrumb')->get('breadcrumb');

      if (isset($config['paths'][$path])) {
        return $config['paths'][$path];
      }
    }
    return array();
  }
}
