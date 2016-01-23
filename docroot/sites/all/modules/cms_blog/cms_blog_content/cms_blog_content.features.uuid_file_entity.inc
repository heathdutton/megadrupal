<?php
/**
 * @file
 * cms_blog_content.features.uuid_file_entity.inc
 */

/**
 * Implements hook_uuid_features_default_file_entities().
 */
function cms_blog_content_uuid_features_default_file_entities() {
  $files = array();

  $files[] = array(
    'filename' => 'unsplash-pineapple.jpeg',
    'uri' => 'public://blog-images/unsplash-pineapple.jpeg',
    'filemime' => 'image/jpeg',
    'filesize' => 163735,
    'status' => 1,
    'type' => 'image',
    'uuid' => '4d781159-cdb5-48a9-9587-baa0e6a903c7',
    'field_file_image_alt_text' => array(),
    'field_file_image_title_text' => array(),
    'rdf_mapping' => array(),
    'metadata' => array(
      'height' => 760,
      'width' => 1140,
    ),
    'alt' => '',
    'title' => '',
    'height' => 760,
    'width' => 1140,
    'uuid_features_packaged_file_path' => 'assets/unsplash-pineapple.jpeg',
    'user_uuid' => 'c155b92b-6053-4816-8abf-401a4fcd0619',
  );
  $files[] = array(
    'filename' => 'unsplash-flower-makro.jpg',
    'uri' => 'public://blog-images/unsplash-flower-makro.jpg',
    'filemime' => 'image/jpeg',
    'filesize' => 240596,
    'status' => 1,
    'type' => 'image',
    'uuid' => '592618fb-18f5-4816-b915-3c58f971b248',
    'field_file_image_alt_text' => array(),
    'field_file_image_title_text' => array(),
    'rdf_mapping' => array(),
    'metadata' => array(
      'height' => 1442,
      'width' => 1920,
    ),
    'alt' => '',
    'title' => '',
    'height' => 1442,
    'width' => 1920,
    'uuid_features_packaged_file_path' => 'assets/unsplash-flower-makro.jpg',
    'user_uuid' => 'c155b92b-6053-4816-8abf-401a4fcd0619',
  );
  return $files;
}
