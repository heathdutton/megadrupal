<?php
/**
 * @file
 * ad.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function ad_filter_default_formats() {
  $formats = array();

  // Exported format: Advertisement format.
  $formats['ad_text_format'] = array(
    'format' => 'ad_text_format',
    'name' => 'Advertisement format',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_html' => array(
        'weight' => -10,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<ad>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
    ),
  );

  return $formats;
}
