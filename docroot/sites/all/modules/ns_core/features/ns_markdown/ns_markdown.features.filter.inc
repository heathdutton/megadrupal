<?php
/**
 * @file
 * ns_markdown.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function ns_markdown_filter_default_formats() {
  $formats = array();

  // Exported format: Markdown.
  $formats['markdown'] = array(
    'format' => 'markdown',
    'name' => 'Markdown',
    'cache' => '1',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'filter_html' => array(
        'weight' => '-47',
        'status' => '1',
        'settings' => array(
          'allowed_html' => '<p> <a> <em> <strong> <ul> <ol> <li> <blockquote> <sub> <sup> <h2> <h3> <h4> <h5> <h6> <br> <code>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'codefilter' => array(
        'weight' => '-46',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_markdown' => array(
        'weight' => '-45',
        'status' => '1',
        'settings' => array(),
      ),
      'pathologic' => array(
        'weight' => '-42',
        'status' => '1',
        'settings' => array(
          'local_paths' => '',
          'protocol_style' => 'full',
          'absolute' => 1,
        ),
      ),
    ),
  );

  return $formats;
}
