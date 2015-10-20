<?php
/**
 * @file
 * Hooks provided by the Mobile Switch module.
 */

/**
 * Alter various configuration settings and values.
 *
 * @param $conf
 *   The associative array contains the values to alter.
 * @param $get
 *   The associative array contains various parameters to help to alter.
 *
 * @ingroup hooks
 */
function hook_mobile_switch_boot_alter(&$conf, $get) {
  // Theme switch from URL.
  // Set users cookie.
  if (isset($_GET['mobile_switch'])) {
    _mobile_switch_block_set_cookie($_GET['mobile_switch']);
    $get['theme_cookie'] = $_GET['mobile_switch'];
  }
  // Provide cookie value.
  $get['theme_cookie'] = _mobile_switch_block_get_cookie();
}
