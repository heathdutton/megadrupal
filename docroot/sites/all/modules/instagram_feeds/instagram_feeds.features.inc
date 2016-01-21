<?php
/**
 * @file
 * instagram_feeds.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function instagram_feeds_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function instagram_feeds_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function instagram_feeds_flag_default_flags() {
  $flags = array();
  // Exported flag: "Flag as Inappropriate".
  $flags['flag_as_inappropriate'] = array(
    'entity_type' => 'node',
    'title' => 'Flag as Inappropriate',
    'global' => 1,
    'types' => array(
      0 => 'instagram_media_item',
    ),
    'flag_short' => 'Flag as Inappropriate',
    'flag_long' => 'Flag as Inappropriate image',
    'flag_message' => 'Flagged as Inappropriate. This image will be hidden in a short time.',
    'unflag_short' => 'Flag as Appropriate',
    'unflag_long' => 'Remove Inappropriate flag for this image.',
    'unflag_message' => 'Flagged as Appropriate. Now this image will not be hidden.',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => -7,
    'show_in_links' => array(
      'teaser' => TRUE,
      'full' => TRUE,
    ),
    'show_as_field' => FALSE,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'instagram_feeds',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

/**
 * Implements hook_image_default_styles().
 */
function instagram_feeds_image_default_styles() {
  $styles = array();

  // Exported image style: instagram_100x100.
  $styles['instagram_100x100'] = array(
    'name' => 'instagram_100x100',
    'effects' => array(
      54 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 100,
          'height' => 100,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'instagram_100x100',
  );

  // Exported image style: instagram_150x150.
  $styles['instagram_150x150'] = array(
    'name' => 'instagram_150x150',
    'effects' => array(
      53 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 150,
          'height' => 150,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'instagram_150x150',
  );

  // Exported image style: instagram_50x50.
  $styles['instagram_50x50'] = array(
    'name' => 'instagram_50x50',
    'effects' => array(
      55 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 50,
          'height' => 50,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'instagram_50x50',
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function instagram_feeds_node_info() {
  $items = array(
    'instagram_feed' => array(
      'name' => t('Instagram Feed'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name of Feed'),
      'help' => '',
    ),
    'instagram_media_item' => array(
      'name' => t('Instagram Media Item'),
      'base' => 'node_content',
      'description' => t('This content type used for downloading images from Instagram via API.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'instagram_url' => array(
      'name' => t('Instagram URL'),
      'base' => 'node_content',
      'description' => t('This content type is used by Instagram Feed importer. Instagram url creates automatically after creating new Instagram Feed node.
If you don\'t know what this, DO NOT create Instagram url manually.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
