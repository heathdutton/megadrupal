<?php
/**
 * @file
 * fancy_content_headline.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function fancy_content_headline_filter_default_formats() {
  $formats = array();

  // Exported format: Headline.
  $formats['fancy_headline'] = array(
    'format' => 'fancy_headline',
    'name' => 'Headline',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_html' => array(
        'weight' => -10,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_url' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'pathologic' => array(
        'weight' => 50,
        'status' => 1,
        'settings' => array(
          'local_paths' => '',
          'protocol_style' => 'full',
        ),
      ),
    ),
  );

  return $formats;
}
