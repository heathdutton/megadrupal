<?php
/**
 * @file
 * html.process.inc
 */

/**
 * Implements hook_process_html().
 */
function wetkit_omega_process_html(&$variables) {
  // Flatten attributes arrays.
  $variables['html_attributes'] = empty($variables['html_attributes_array']) ? '' : drupal_attributes($variables['html_attributes_array']);
}
