<?php


class entityaspect_PageInfoCache {

  protected $pages = array();
  protected $active;
  protected $entitySystem;

  function __construct($entity_system) {
    $this->entitySystem = $entity_system;
  }

  function reset() {
    $this->pages = array();
  }

  function pageInfo($entity_type, $entity, $route) {
    $etid = $this->entitySystem->entityToId($entity_type, $entity);
    if (!isset($this->pages[$route][$etid])) {
      $this->pages[$route][$etid] = _entityaspect_page_info_build($entity_type, $entity, $route, $etid);
    }
    return $this->pages[$route][$etid];
  }

  function page($entity_type, $entity, $route) {
    $page_info = $this->pageInfo($entity_type, $entity, $route);
    $this->active = $page_info;
    return $page_info->page();
  }

  function hook_pageapi($api) {
    if (!empty($this->active)) {
      $page_info = $this->active;
      $page_info->hook_pageapi($api);
    }
  }

  function hook_menu_local_tasks_alter(&$data, $router_item, $root_path) {

    if (!is_array($data['tabs']) || empty($data['tabs'])) {
      return;
    }

    if (!is_array($router_item['load_functions'])) {
      return;
    }

    foreach ($router_item['load_functions'] as $pos => $function) {
      $etid = $router_item['original_map'][$pos];
      break;
    }

    if (!isset($etid)) {
      return;
    }

    // Process each tab group
    foreach ($data['tabs'] as &$tabs) {
      if (is_array($tabs['output'])) {

        // Process each tab individually
        foreach ($tabs['output'] as $i => &$item) {
          $route = $item['#link']['path'];
          if (!empty($this->pages[$route][$etid])) {
            $this->pages[$route][$etid]->alterLocalTask($item);
          }
        }

        // Rearrange tabs.
        $this->tabsStableSort($tabs['output']);
      }
    }
  }

  function hook_preprocess_page(&$vars) {

    // Specific stuff for the active page.
    if (!empty($this->active)) {
      $page_info = $this->active;
      $page_info->hook_preprocess_page($vars);
    }
  }

  protected function tabsStableSort(&$tabs) {
    $sorted = array();
    foreach ($tabs as $i => $item) {
      $path = $item['#link']['path'];
      $weight = $item['#link']['weight'];

      if ($weight === FALSE) {
        unset($vars['tabs'][$k][$i]);
      }
      else {
        $sorted[$weight][] = $item;
      }
    }
    ksort($sorted);
    $tabs = array();
    foreach ($sorted as $items) {
      foreach ($items as $item) {
        $tabs[] = $item;
      }
    }
  }
}
