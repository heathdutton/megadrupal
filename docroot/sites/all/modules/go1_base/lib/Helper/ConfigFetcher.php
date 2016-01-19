<?php
namespace Drupal\go1_base\Helper;

/**
 * Usage:
 *
 *  go1_container('helper.config_fetcher')
 *    ->getItems('go1_base', 'services', 'services', TRUE)
 *  ;
 *
 * @todo  Test me
 * @todo  Remove duplication code — go1_modules('go1_base', …)
 * @todo  Support expression_language:evaluate() — check \Drupal\go1_base\Hook\BlockInfo
 */
class ConfigFetcher {
  public function getItems($module, $id, $key, $include_base = FALSE, $reset = FALSE) {
    $o = array(
      'ttl' => '+ 1 year',
      'id' => "GO1Config:{$module}:{$id}:{$key}:" . ($include_base ? 1 : 0),
      'reset' => $reset,
    );

    return go1_cache($o, array($this, 'fetchItems'), array($module, $id, $key, $include_base));
  }

  public function fetchItems($module, $id, $key, $include_base) {
    $modules = go1_modules($module, $id);

    if ($include_base) {
      $modules = array_merge(array($module), $modules);
    }

    $items = array();
    foreach ($modules as $module_name) {
      $items = array_merge($items, go1_config($module_name, $id)->get($key));
    }

    return $items;
  }

  public function getItem($module, $id, $key, $item_key, $include_base = FALSE, $reset = FALSE) {
    $o = array(
      'ttl' => '+ 1 year',
      'id' => "GO1Config:{$module}:{$id}:{$key}:{$item_key}:" . ($include_base ? 1 : 0),
      'reset' => $reset,
    );

    return go1_cache($o, array($this, 'fetchItem'), array($module, $id, $key, $item_key, $include_base));
  }

  public function fetchItem($module, $id, $key, $item_key, $include_base) {
    if ($items = $this->getItems($module, $id, $key, $include_base)) {
      if (!empty($items[$item_key])) {
        return $items[$item_key];
      }
    }
  }
}
