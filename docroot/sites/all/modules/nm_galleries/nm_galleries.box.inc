<?php
/**
 * @file
 * nm_galleries.box.inc
 */

/**
 * Implements hook_default_box().
 */
function nm_galleries_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'nm_galleries_featured_gallery';
  $box->plugin_key = 'gallery';
  $box->title = '';
  $box->description = 'Featured Gallery';
 
  //get nid of Sample Gallery
  $gallery = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type' , 'nm_gallery')
    ->condition('title' , 'Sample Gallery')
    ->execute()->fetchAssoc();

  $box->options = array(
    'gallery' => $gallery['nid'],
  );
  $export['nm_galleries_featured_gallery'] = $box;

  return $export;
}
