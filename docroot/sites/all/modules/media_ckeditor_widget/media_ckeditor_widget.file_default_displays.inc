<?php
/**
 * @file
 * web3.file_default_displays.inc
 */

/**
 * Implements hook_file_default_displays().
 */
function media_ckeditor_widget_file_default_displays() {
  $export = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__default__media_oembed';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '640',
    'height' => '360',
  );
  $export['image__default__media_oembed'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__default__file_field_image';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
    'image_link' => '',
  );
  $export['image__default__file_field_image'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__default__file_field_file_default';
  $file_display->weight = 50;
  $file_display->status = TRUE;
  $file_display->settings = array();
  $export['image__default__file_field_file_default'] = $file_display;

  //Image WYSIWYG
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__wysiwyg__file_field_image';
  $file_display->weight = -44;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
    'image_link' => '',
  );
  $export['image__wysiwyg__file_field_image'] = $file_display;


  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__wysiwyg__media_oembed_thumbnail';
  $file_display->weight = -49;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
  );
  $export['image__wysiwyg__media_oembed_thumbnail'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'image__wysiwyg__file_field_media_large_icon';
  $file_display->weight = -30;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => '',
  );
  $export['image__wysiwyg__file_field_media_large_icon'] = $file_display;

  // Audio WYSIWYG
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__wysiwyg__media_oembed_thumbnail';
  $file_display->weight = -49;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
  );
  $export['audio__wysiwyg__media_oembed_thumbnail'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__wysiwyg__file_field_media_large_icon';
  $file_display->weight = -40;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => '',
  );
  $export['audio__wysiwyg__file_field_media_large_icon'] = $file_display;

  // Video WYSIWYG
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__wysiwyg__file_field_media_large_icon';
  $file_display->weight = -30;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => '',
  );
  $export['video__wysiwyg__file_field_media_large_icon'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__wysiwyg__media_oembed_thumbnail';
  $file_display->weight = -50;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
  );
  $export['video__wysiwyg__media_oembed_thumbnail'] = $file_display;

  // Document WYSIWYG
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'document__wysiwyg__file_field_media_large_icon';
  $file_display->weight = -50;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => '',
  );
  $export['document__wysiwyg__file_field_media_large_icon'] = $file_display;

  return $export;
}
