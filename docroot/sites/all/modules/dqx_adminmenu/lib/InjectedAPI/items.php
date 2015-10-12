<?php


class dqx_adminmenu_InjectedAPI_items {

  protected $items;
  protected $cache = array();

  function __construct(&$items, $cache) {
    $this->items =& $items;
    $this->cache = $cache;
  }

  function addTree($rootPath) {
    $items = $this->cache->getTreeItems($rootPath);
    foreach ($items as $key => $item) {
      $this->items[$key] = $item;
    }
    return $items;
  }

  function forceShowItem($path) {
    $item = $this->cache->forceItem($path);
    if (!empty($item)) {
      $this->items[$path] = $item;
    }
  }
}
