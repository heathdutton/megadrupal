<?php

class menupoly_BlockInfo {

  /**
   * @return array[]
   *   Array of block definitions.
   */
  function hook_block_info() {
    $definitions = $this->_getBlockDefinitions();
    $blocks = array();
    foreach ($definitions as $module => $module_definitions) {
      foreach ($module_definitions as $key => $config) {
        $delta = "$module-$key";
        $blocks[$delta] = array(
          'info' => "Menupoly [$delta]",
          'cache' => DRUPAL_NO_CACHE,
        );
        if (isset($config['block']) && is_array($config['block'])) {
          $blocks[$delta] += $config['block'];
        }
      }
    }
    return $blocks;
  }

  /**
   * @param string $delta
   *
   * @return array[]|NULL
   */
  function hook_block_view($delta = '') {
    list($module, $key) = explode('-', $delta);
    $f = $module . '_menupoly';
    if (!function_exists($f)) {
      return NULL;
    }
    $result = $f();
    if (!isset($result[$key])) {
      return NULL;
    }
    $config = $result[$key];
    return $config + array(
      'content' => array(
        '#theme' => 'menupoly',
        '#menupoly' => $config,
      ),
    );
  }

  /**
   * @return array[]
   */
  protected function _getBlockDefinitions() {
    $definitions = array();
    foreach (module_implements('menupoly') as $module) {
      $function = $module . '_menupoly';
      $result = $function();
      foreach ($result as $key => $config) {
        $definitions[$module][$key] = $config;
      }
    }
    return $definitions;
  }
}
