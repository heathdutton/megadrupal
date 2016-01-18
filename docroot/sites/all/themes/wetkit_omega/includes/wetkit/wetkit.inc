<?php

/**
 * @file
 * Main extension file for the 'layouts' extension.
 */

/**
 * Implements hook_extension_EXTENSION_registry_alter().
 */
function wetkit_omega_extension_wetkit_theme_registry_alter(&$registry) {
  $registry['page']['includes'][] = drupal_get_path('theme', 'wetkit_omega') . '/includes/wetkit/wetkit.inc';
  $registry['page']['process functions'][] = 'wetkit_omega_extension_wetkit_process_layout';
}

/**
 * Implements hook_extension_EXTENSION_process_layout().
 */
function wetkit_omega_extension_wetkit_process_layout(&$variables) {

}
