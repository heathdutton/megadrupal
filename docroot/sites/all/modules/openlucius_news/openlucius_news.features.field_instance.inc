<?php
/**
 * @file
 * openlucius_news.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function openlucius_news_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-ol_news-body'
  $field_instances['node-ol_news-body'] = array(
    'bundle' => 'ol_news',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'node-ol_news-field_shared_files'
  $field_instances['node-ol_news-field_shared_files'] = array(
    'bundle' => 'ol_news',
    'comment_alter' => 0,
    'comment_alter_hide' => 0,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'itweak_upload',
        'settings' => array(
          'files_display_mode' => 3,
          'gallery_limit' => '',
          'gallery_type' => 'itu',
          'image_link_mode' => 'lightbox2grouped',
          'mime_icon_directory' => 'profiles/openlucius/modules/contrib/itweak_upload/icons/itu/16',
          'open_image_style' => '_original',
          'show_caption' => FALSE,
          'show_title' => TRUE,
          'thumbnail_style' => 'AttachmentThumbnail',
        ),
        'type' => 'itu_file_table',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_shared_files',
    'label' => 'Files',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'jpg jpeg gif png txt doc docx xls xlsx pdf ppt pptx pps ppsx odt ods odp mp3 mpeg avi zip psd eps',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'dragndrop_upload_file',
      'settings' => array(
        'allow_replace' => 0,
        'droppable_area_text' => 'Drop files here to upload',
        'media_browser' => 0,
        'multiupload' => 1,
        'progress_indicator' => 'throbber',
        'standard_upload' => 1,
        'upload_button_text' => 'Upload',
        'upload_event' => 'auto',
      ),
      'type' => 'dragndrop_upload_file',
      'weight' => -2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Files');

  return $field_instances;
}
