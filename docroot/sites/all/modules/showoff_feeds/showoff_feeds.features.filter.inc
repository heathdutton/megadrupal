<?php
/**
 * @file
 * showoff_feeds.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function showoff_feeds_filter_default_formats() {
  $formats = array();

  // Exported format: Full HTML.
  $formats['full_html'] = array(
    'format' => 'full_html',
    'name' => 'Full HTML',
    'cache' => '1',
    'status' => '1',
    'weight' => '1',
    'filters' => array(),
  );

  return $formats;
}
