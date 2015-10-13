<?php
/**
 * @file
 * page.process.inc
 */

/**
 * Implements hook_process_page().
 */
function wetkit_omega_process_page(&$variables) {
  // Flatten attributes arrays.
  $variables['footer_attributes'] = empty($variables['footer_attributes_array']) ? '' : drupal_attributes($variables['footer_attributes_array']);
}
