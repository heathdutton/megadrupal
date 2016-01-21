<?php
/**
 * @file
 * codebook_wysiwyg.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function codebook_wysiwyg_filter_default_formats() {
  $formats = array();

  // Exported format: Codebook.
  $formats['codebook'] = array(
    'format' => 'codebook',
    'name' => 'Codebook',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'media_filter' => array(
        'weight' => 2,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
