<?php
namespace Drupal\go1_base\Hook;

/**
 * Details for go1_base_flush_caches().
 */
class FlushCache {
  public function execute() {
    $this->flushAPCData();
    $this->flushTaggedCacheData();
    $this->refreshCachedModules();
    $this->fixModuleWeight();
  }

  private function flushTaggedCacheData() {
    go1_container('wrapper.db')->delete('go1_base_cache_tag')->execute();
  }

  private function flushAPCData() {
    if (function_exists('apc_clear_cache')) {
      apc_clear_cache('user');
    }
  }

  private function refreshCachedModules() {
    go1_modules('go1_base', TRUE);
  }

  /**
   * Update module's weight value in system table.
   */
  public function fixModuleWeight() {
    foreach (system_list('module_enabled') as $module_name => $module_info) {
      if (isset($module_info->info['weight'])) {
        $this->resolveModuleWeight($module_name, $module_info->info['weight']);
      }
    }
  }

  public function resolveModuleWeight($module_name, $weight) {
    if (is_numeric($weight)) {
      go1_container('wrapper.db')
        ->update('system')
        ->condition('name', $module_name)
        ->fields(array('weight' => $weight))
        ->execute()
      ;
    }
  }
}
