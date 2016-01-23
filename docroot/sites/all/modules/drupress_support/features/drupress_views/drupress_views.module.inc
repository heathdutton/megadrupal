<?php
/**
 * @file
 * Custom code, hooks etc for the Drupress Views feature.
 */

/**
 * Implements hook_views_views_default_views_alter().
 */
function drupress_views_views_default_views_alter(&$views) {
  // Enable the archive and taxonomy_term views that comes (disabled) with views.module.
  if (isset($views['archive'])) {
    $views['archive']->disabled = FALSE;
  }
  if (isset($views['taxonomy_term'])) {
    $views['taxonomy_term']->disabled = FALSE;
  }
}
