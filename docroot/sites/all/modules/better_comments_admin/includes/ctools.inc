<?php
/**
 * @file
 * Chaos tool suite (ctools) module integration.
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function better_comments_admin_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == 'page_manager' && $api =='pages_default') {
    return array(
      'version' => 1
    );
  }
}
