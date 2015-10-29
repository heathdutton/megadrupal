<?php
/**
 * @file
 * node.process.inc
 */


/**
 * Implements hook_process_node().
 */
function wetkit_omega_process_node(&$variables) {
  // Flatten the additional wrapper attributes array.
  $variables['header_attributes'] = empty($variables['header_attributes_array']) ? '' : drupal_attributes($variables['header_attributes_array']);
  $variables['footer_attributes'] = empty($variables['footer_attributes_array']) ? '' : drupal_attributes($variables['footer_attributes_array']);
  $variables['links_attributes'] = empty($variables['links_attributes_array']) ? '' : drupal_attributes($variables['links_attributes_array']);
}
