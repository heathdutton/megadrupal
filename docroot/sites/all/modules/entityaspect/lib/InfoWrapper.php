<?php


class entityaspect_InfoWrapper {

  protected $data;

  /**
   * @param array $data
   *   Information collected with hook_entityaspect()
   */
  function __construct(array $data) {
    $this->data = $data;
  }

  /**
   * Called by implementation of hook_entity_info_alter()
   */
  function hook_entity_info_alter(&$entity_info) {
    foreach ($this->data['view_modes'] as $entity_type => $view_modes) {
      foreach ($view_modes as $view_mode => $info) {
        $entity_info[$entity_type]['view modes'][$view_mode] = $info;
      }
    }
  }

  /**
   * Called by implementation of hook_menu()
   */
  function hook_menu() {
    return $this->data['hook_menu'];
  }

  /**
   * Called by implementation of hook_menu_alter()
   */
  function hook_menu_alter(&$items) {
    // TODO: Is this a sufficient way to override items?
    foreach ($this->data['hook_menu_alter'] as $route => $item) {
      if (isset($items[$route])) {
        unset($item['type']);
        if (isset($items[$route]['type'])) {
          $item['type'] = $items[$route]['type'];
        }
        unset($items[$route]);
      }
      $items[$route] = $item;
    }
  }
}
