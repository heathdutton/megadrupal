<?php
namespace Drupal\go1_ui\Controller\Reports;

class Entity_Templates {
  public function render() {
    $rows = array();

    foreach (go1_modules('go1_base', 'entity_template') as $module) {
      foreach (go1_config($module, 'entity_template')->get('entity_templates') as $entity_type => $entity_config) {
        foreach ($entity_config as $bundle => $bundle_config) {
          foreach ($bundle_config as $view_mode => $config) {
            $attached = array();
            if (isset($config['attached'])) {
              $attached = $config['attached'];
              unset($config['attached']);
            }

            $blocks = array();
            if (isset($config['blocks'])) {
              $blocks = $config['blocks'];
              unset($config['blocks']);
            }

            $rows[] = array($entity_type, $bundle, $view_mode, go1dr($config), go1dr($attached), go1dr($blocks));
          }
        }
      }
    }

    return array('#theme' => 'table',
      '#header' => array(
        array('data' => 'Entity', 'width' => '100px'),
        array('data' => 'Bundle', 'width' => '100px'),
        array('data' => 'View Mode', 'width' => '100px'),
        array('data' => 'Config'),
        array('data' => 'Attached'),
        array('data' => 'Block'),
      ),
      '#rows' => $rows,
      '#empty' => 'Empty');
  }
}
