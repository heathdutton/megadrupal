<?php
/**
 * @file
 * Document menu_rewrite hooks.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define menu rewrite rules.
 *
 * @return
 *   An associative array of menu rewrite rules. The key being the original
 *   path and the value being the path to rewrite it to. The path to rewrite
 *   may contain children which will also be moved.
 */
function hook_menu_rewrite_rule() {
  // Make 'Development' a primary item.
  return array(
    'admin/config/development' => 'admin/development',
  );
}

/**
 * Alter menu items after they have been rewritten.
 *
 * @see hook_menu_alter()
 */
function hook_menu_rewrite_alter(&$items) {
  // Ensure 'Modules' menu item shows up first.
  $items['admin/modules']['weight'] = -11;
}

/**
 * @} End of "addtogroup hooks".
 */
