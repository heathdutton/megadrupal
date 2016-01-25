<?php

/**
 * @file
 * Check for dependencies and alter alias handling.
 */

/**
 * Implements hook_drush_load().
 */
function vagrant_drush_load() {
  $alias = drush_get_context('alias');
  // Ensure we're using a drush-vagrant group alias
  if (isset($alias['#name']) &&
      !isset($alias['type']) &&
      isset($alias['project-path'])) {
    // Remove the project alias from the site-list
    if (array_shift($alias['site-list']) == $alias['#name'] . '.project') {
      drush_set_context('alias', $alias);
    }
  }
  return TRUE;
}
