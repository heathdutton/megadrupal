<?php
/**
 * @file
 * Hooks provided by the Site Install Hooks API library.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_site_pre_install().
 *
 * This hook is invoked immediately before install_profile_modules() in Drupal
 * core.
 */
function hook_site_pre_install() {
  // Initialize a variable that controls how a module gets installed down
  // the road
  variable_set('some_var', 1);
}

/**
 * Implements hook_site_post_install().
 *
 * This hook is invoked immediately before install_finished() in Drupal
 * core.
 */
function hook_site_post_install() {
  // Force the site name to a constant value regardless of what the user
  // set during installation.
  variable_set('site_name', 'My super awesome site');
}

/**
 * Implements hook_module_implements_alter().
 *
 * Demonstrates that you can use this hook to alter the order that post-install
 * hooks are invoked.
 */
function hook_module_implements_alter(&$implementations, $hook) {
  // Move this module to be invoked last in the chain
  if ($hook == 'site_post_install') {
    $group = $implementations['my_module'];
    unset($implementations['my_module']);
    $implementations['my_module'] = $group;
  }
}

/**
 * @} End of "addtogroup hooks".
 */