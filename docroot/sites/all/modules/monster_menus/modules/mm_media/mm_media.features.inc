<?php
/**
 * @file
 * mm_media.features.inc
 */

/**
 * Implements hook_node_info().
 */
function mm_media_node_info() {
  return array(
    'gallery' => array(
      'name' => t('Gallery'),
      'base' => 'mm_media_gallery',
      'description' => mm_media_help('node/add#gallery'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'mm_media' => array(
      'name' => t('File Upload'),
      'base' => 'node_content',
      'description' => t('Upload images, PDFs, audio and other media files (after uploading files, you can insert them in articles).'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
}
