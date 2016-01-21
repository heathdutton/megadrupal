<?php
/**
 * @file
 * ns_text.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function ns_text_filter_default_formats() {
  $formats = array();

  // Exported format: Full HTML.
  $formats['full_html'] = array(
    'format' => 'full_html',
    'name' => 'Full HTML',
    'cache' => '1',
    'status' => '1',
    'weight' => '-9',
    'filters' => array(
      'filter_htmlcorrector' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  // Exported format: HTML editor.
  $formats['html_editor'] = array(
    'format' => 'html_editor',
    'name' => 'HTML editor',
    'cache' => '1',
    'status' => '1',
    'weight' => '-10',
    'filters' => array(
      'filter_html' => array(
        'weight' => '-10',
        'status' => '1',
        'settings' => array(
          'allowed_html' => '<p> <a> <em> <strong> <ul> <ol> <li> <blockquote> <sub> <sup> <h2> <h3> <h4> <h5> <h6> <br>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(),
      ),
      'media_filter' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(),
      ),
      'pathologic' => array(
        'weight' => '10',
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
