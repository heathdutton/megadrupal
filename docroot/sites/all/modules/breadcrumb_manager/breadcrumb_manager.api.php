<?php

/**
 * @file
 * Describe hooks provided by the Breadcrumb Manager module.
 */

/**
 * @defgroup breadcrumb_manager_hooks Breadcrumb Manager hooks
 * @{
 * Hooks that can be implemented by other modules in order to implement the
 * Breadcrumb Manager API.
 */

/**
 * Allow other modules to alter the generated breadcrumb.
 *
 * @param $breadcrumb
 * @param $current_path
 */
function hook_breadcrumb_alter(&$breadcrumb, $current_path) {
  if ($current_path == 'news') {
    $breadcrumb = array(
      l(t('Home'), '<front>'),
      'News'
    );
  }
}

/**
 * @}
 */

