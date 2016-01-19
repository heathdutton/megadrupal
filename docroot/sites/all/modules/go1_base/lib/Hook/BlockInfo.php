<?php
namespace Drupal\go1_base\Hook;

class BlockInfo {
  public function import() {
    $info = array();
    foreach (go1_modules('go1_base', 'blocks') as $module) {
      $info += $this->importResource($module);
    }
    return $info;
  }

  private function importResource($module) {
    $info = array();
    foreach (go1_config($module, 'blocks')->get('blocks') as $k => $block) {
      $cache = DRUPAL_CACHE_PER_ROLE;
      if (!empty($block['cache'])) {
        $cache = go1_eval($block['cache']);
      }

      $info["{$module}|{$k}"] = array(
        'info' => empty($block['info']) ? $k : $block['info'],
        'cache' => $cache,
      );
    }
    return $info;
  }
}
