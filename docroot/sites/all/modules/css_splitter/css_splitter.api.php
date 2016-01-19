<?php
/**
 * @file
 * Hooks provided by the CSS Splitter module.
 */

/**
 *@addtogroup hooks
 * @{
 */

/**
 * Allow modules to define css groups.
 *
 * @return
 *   An array of group descriptions. Each value is an associative array with
 *   the group name as the key and the group weight as the value.
 */
function hook_css_splitter_groups_info() {
  return array(
    'system' => CSS_SYSTEM,
    'default' => CSS_DEFAULT,
    'theme' => CSS_THEME
  );
}

/**
 * Allow modules to alter css group information.
 *
 * This hook is not stricly very useful.
 *
 * @param $css_groups
 *   An array of group descriptions. Each value is an associative array with
 *   the group name as the key and the group weight as the value.
 */
function hook_css_splitter_groups_alter(&$css_groups) {
  $css_groups['example'] = 42;
}
