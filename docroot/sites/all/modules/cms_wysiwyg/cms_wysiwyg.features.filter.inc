<?php
/**
 * @file
 * cms_wysiwyg.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function cms_wysiwyg_filter_default_formats() {
  $formats = array();

  // Exported format: WYSIWYG Full HTML.
  $formats['wysiwyg_full'] = array(
    'format' => 'wysiwyg_full',
    'name' => 'WYSIWYG Full HTML',
    'cache' => 1,
    'status' => 1,
    'weight' => -9,
    'filters' => array(
      'media_filter' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -45,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  // Exported format: WYSIWYG Simple.
  $formats['wysiwyg_simple'] = array(
    'format' => 'wysiwyg_simple',
    'name' => 'WYSIWYG Simple',
    'cache' => 1,
    'status' => 1,
    'weight' => -10,
    'filters' => array(
      'filter_html' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd> <p> <br>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'media_filter' => array(
        'weight' => -43,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_autop' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  return $formats;
}
