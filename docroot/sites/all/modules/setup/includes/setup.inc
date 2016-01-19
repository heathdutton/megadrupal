<?php
/**
 * @file
 * Setup module integration.
 */

/**
 * Implements hook_setup_styles().
 */
function setup_setup_styles() {
  $styles = array();

  // Drupal install.php inspired style.
  $styles['drupalsetup'] = array(
    'css' => array(
      drupal_get_path('module', 'setup') . '/styles/drupalsetup/drupalsetup.css',
    ),
  );

  // Empty style.
  $styles['none'] = array();

  return $styles;
}
