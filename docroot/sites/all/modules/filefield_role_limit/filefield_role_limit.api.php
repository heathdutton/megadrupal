<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup filefield_role_limit_hooks Filefield Role Limit's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend Filefield
 * Role Limit module functionalities.
 */

/**
 * Alter the current supported widgets.
 *
 * This hook is used to add Filefield widgets to the current supported list.
 *
 * @param array $types
 *   The current supported widget list.
 *
 * @ingroup filefield_role_limit_hooks
 * @ingroup hooks
 */
function hook_filefield_role_limit_supported_widgets_alter(&$types) {
  $types[] = 'mycustom_widget';
}

/**
 * @}
 */