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
    'filesize' => 3170083,
    'status' => 1,
    'type' => 'image',
    'uuid' => '4d781159-cdb5-48a9-9587-baa0e6a903c7',
    'field_file_image_alt_text' => array(),
    'field_file_image_title_text' => array(),
    'rdf_mapping' => array(),
    'metadata' => array(
      'height' => 3553,
      'width' => 5330,
    ),
    'alt' => '',
    'title' => '',
    'height' => 3553,
    'width' => 5330,
    'uuid_features_packaged_file_path' => 'assets/unsplash-pineapple.jpeg',
    'user_uuid' => '27324dc9-31eb-416e-831a-8de6ede64374',
  );
  $files[] = array(
    'filename' => 'girls-685787_1280.jpg',
    'uri' => 'public://blog-images/girls-685787_1280.jpg',
    'filemime' => 'image/jpeg',
    'filesize' => 311588,
    'status' => 1,
    'type' => 'image',
    'uuid' => 'a068782c-872a-47a0-b230-abce4c10d585',
    'field_file_image_alt_text' => array(),
    'field_file_image_title_text' => array(),
    'rdf_mapping' => array(),
    'metadata' => array(
      'height' => 1005,
      'width' => 1280,
    ),
    'alt' => '',
    'title' => '',
    'height' => 1005,
    'width' => 1280,
    'uuid_features_packaged_file_path' => 'assets/girls-685787_1280.jpg',
    'user_uuid' => '27324dc9-31eb-416e-831a-8de6ede64374',
  );
  return $files;
}
